#include <stdio.h>
#include <string.h>

// Reference:
// https://github.com/Thinkerfans/lib-mp4v2/blob/master/mp4v2/mp4record.c
// https://github.com/lieff/minih264/blob/master/minih264e_test.c

#define H264E_MAX_THREADS 0
#define MINIH264_IMPLEMENTATION
#include "minih264e.h"
#include "mp4v2/mp4v2.h"

#include "h264_mp4_encoder.h"

#define ALIGNED_ALLOC(n, size) aligned_alloc(n, (size + n - 1) / n * n)

#define TIMESCALE 90000

enum class NALU
{
  SPS = 0x67,
  PPS = 0x68,
  I = 0x65,
  P = 0x61,
  INVALID = 0x00,
};

#define CHECK(expr)                                           \
  do                                                          \
  {                                                           \
    if (!(expr))                                              \
    {                                                         \
      printf("ERROR: %s (%d)\n    EXPR: %s\n    FUNC: %s\n",  \
             __FILE__, __LINE__, #expr, __PRETTY_FUNCTION__); \
      abort();                                                \
    }                                                         \
  } while (false)

class h264_mp4_encoder_private
{
public:
  h264_mp4_encoder *encoder = nullptr;
  MP4FileHandle mp4 = nullptr;
  MP4TrackId video_track = 0;

  int frame = 0;
  H264E_persist_t *enc = nullptr;
  H264E_scratch_t *scratch = nullptr;
  uint8_t *rgba_to_yuv_buffer = nullptr;

  static void nalu_callback(const uint8_t *nalu_data, int sizeof_nalu_data, void *token);
};

void h264_mp4_encoder_private::nalu_callback(
    const uint8_t *nalu_data, int sizeof_nalu_data, void *token)
{
  h264_mp4_encoder_private *encoder_private = (h264_mp4_encoder_private *)token;
  h264_mp4_encoder *encoder = encoder_private->encoder;

  uint8_t *data = const_cast<uint8_t *>(nalu_data - STARTCODE_4BYTES);
  const int size = sizeof_nalu_data + STARTCODE_4BYTES;

  CHECK(size >= 5);
  CHECK(data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 1);

  NALU index = (NALU)data[4];

  if (encoder->options.debug)
  {
    printf("nalu=");
    switch (index)
    {
    case NALU::SPS:
      printf("SPS");
      break;
    case NALU::PPS:
      printf("PPS");
      break;
    case NALU::I:
      printf("I");
      break;
    case NALU::P:
      printf("P");
      break;
    }

    printf(", hex=");
    for (size_t i = 0; i < sizeof_nalu_data && i < 16; ++i)
    {
      if (i != 0)
      {
        printf(" ");
      }
      printf("%02X", nalu_data[i]);
      if (i == 15 && sizeof_nalu_data != 16)
      {
        printf("...");
      }
    }
    printf(", bytes=%d\n", sizeof_nalu_data);
  }

  switch (index)
  {
  case NALU::SPS:
    if (encoder_private->video_track == MP4_INVALID_TRACK_ID)
    {
      encoder_private->video_track = MP4AddH264VideoTrack(
          encoder_private->mp4,
          TIMESCALE,
          TIMESCALE / encoder->options.frame_rate,
          encoder->options.width,
          encoder->options.height,
          data[5], // sps[1] AVCProfileIndication
          data[6], // sps[2] profile_compat
          data[7], // sps[3] AVCLevelIndication
          3);      // 4 bytes length before each NAL unit
      CHECK(encoder_private->video_track != MP4_INVALID_TRACK_ID);
      //  Simple Profile @ Level 3
      MP4SetVideoProfileLevel(encoder_private->mp4, 0x7F);
    }
    MP4AddH264SequenceParameterSet(encoder_private->mp4, encoder_private->video_track, nalu_data, sizeof_nalu_data);
    break;
  case NALU::PPS:
    MP4AddH264PictureParameterSet(encoder_private->mp4, encoder_private->video_track, nalu_data, sizeof_nalu_data);
    break;
  case NALU::I:
  case NALU::P:
    data[0] = sizeof_nalu_data >> 24;
    data[1] = sizeof_nalu_data >> 16;
    data[2] = sizeof_nalu_data >> 8;
    data[3] = sizeof_nalu_data & 0xff;
    CHECK(MP4WriteSample(encoder_private->mp4, encoder_private->video_track, data, size, MP4_INVALID_DURATION, 0, 1));
    break;
  default:
    CHECK(false && "Unknown frame type");
  }
}

h264_mp4_encoder::h264_mp4_encoder(const h264_mp4_encoder_options &options) : options(options),
                                                                              private_(new h264_mp4_encoder_private())
{
  CHECK(private_);
  private_->encoder = this;

  CHECK(options.on_data_callback != nullptr);
  CHECK(options.width > 0);
  CHECK(options.height > 0);
  CHECK(options.frame_rate > 0);
  CHECK(options.quantization_parameter >= 10 && options.quantization_parameter <= 51);
  CHECK(options.speed <= 10);

  private_->mp4 = MP4Create("output.mp4", 0);
  CHECK(private_->mp4 != MP4_INVALID_FILE_HANDLE);
  MP4SetTimeScale(private_->mp4, TIMESCALE);

  H264E_create_param_t create_param;
  memset(&create_param, 0, sizeof(create_param));
  create_param.enableNEON = 1;
#if H264E_SVC_API
  create_param.num_layers = 1;
  create_param.inter_layer_pred_flag = 1;
  create_param.inter_layer_pred_flag = 0;
#endif
  create_param.gop = options.group_of_pictures;
  create_param.height = options.height;
  create_param.width = options.width;
  create_param.fine_rate_control_flag = 0;
  create_param.const_input_flag = 1;
  create_param.vbv_size_bytes = 100000 / 8;
  create_param.temporal_denoise_flag = options.temporal_denoise;

  int sizeof_persist = 0;
  int sizeof_scratch = 0;
  CHECK(H264E_sizeof(&create_param, &sizeof_persist, &sizeof_scratch) == H264E_STATUS_SUCCESS);
  if (options.debug)
  {
    printf("sizeof_persist=%d, sizeof_scratch=%d\n", sizeof_persist, sizeof_scratch);
  }

  private_->enc = (H264E_persist_t *)ALIGNED_ALLOC(64, sizeof_persist);
  private_->scratch = (H264E_scratch_t *)ALIGNED_ALLOC(64, sizeof_scratch);

  CHECK(H264E_init(private_->enc, &create_param) == H264E_STATUS_SUCCESS);
}

h264_mp4_encoder::~h264_mp4_encoder()
{
  MP4Close(private_->mp4, 0);
  free(private_->enc);
  free(private_->scratch);
  free(private_->rgba_to_yuv_buffer);
  delete private_;
}

void h264_mp4_encoder::add_frame_yuv(uint8_t *yuv, const uint32_t size)
{
  CHECK(size == options.width * options.height * 3 / 2 /*YUV*/);

  H264E_io_yuv_t yuv_planes;
  yuv_planes.yuv[0] = yuv;
  yuv_planes.stride[0] = options.width;
  yuv_planes.yuv[1] = yuv + options.width * options.height;
  yuv_planes.stride[1] = options.width / 2;
  yuv_planes.yuv[2] = yuv + options.width * options.height * 5 / 4;
  yuv_planes.stride[2] = options.width / 2;

  H264E_run_param_t run_param;
  memset(&run_param, 0, sizeof(run_param));
  run_param.frame_type = 0;
  run_param.encode_speed = options.speed;
  run_param.desired_nalu_bytes = options.desired_nalu_bytes;

  if (options.kbps)
  {
    run_param.desired_frame_bytes = options.kbps * 1000 / 8 / options.frame_rate;
    run_param.qp_min = 10;
    run_param.qp_max = 50;
  }
  else
  {
    run_param.qp_min = run_param.qp_max = options.quantization_parameter;
  }

  run_param.nalu_callback_token = this->private_;
  run_param.nalu_callback = &h264_mp4_encoder_private::nalu_callback;

  int sizeof_coded_data = 0;
  uint8_t *coded_data = nullptr;
  CHECK(H264E_encode(
            private_->enc,
            private_->scratch,
            &run_param,
            &yuv_planes,
            &coded_data,
            &sizeof_coded_data) == H264E_STATUS_SUCCESS);

  if (options.debug)
  {
    printf("frame=%d, bytes=%d\n", private_->frame, sizeof_coded_data);
  }
  ++private_->frame;
}

void h264_mp4_encoder::add_frame_rgba(const uint8_t *rgba, const uint32_t size)
{
  CHECK(size == options.width * options.height * 4 /*RGBA*/);

  size_t yuv_size = options.width * options.height * 3 / 2;

  if (!private_->rgba_to_yuv_buffer)
  {
    private_->rgba_to_yuv_buffer = (uint8_t *)malloc(yuv_size);
  }

  // https://stackoverflow.com/questions/9465815/rgb-to-yuv420-algorithm-efficiency
  size_t image_size = options.width * options.height;
  size_t upos = image_size;
  size_t vpos = upos + upos / 4;
  size_t i = 0;

  for (size_t line = 0; line < options.height; ++line)
  {
    if (!(line % 2))
    {
      for (size_t x = 0; x < options.width; x += 2)
      {
        uint8_t r = rgba[4 * i];
        uint8_t g = rgba[4 * i + 1];
        uint8_t b = rgba[4 * i + 2];

        private_->rgba_to_yuv_buffer[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;

        private_->rgba_to_yuv_buffer[upos++] = ((-38 * r + -74 * g + 112 * b) >> 8) + 128;
        private_->rgba_to_yuv_buffer[vpos++] = ((112 * r + -94 * g + -18 * b) >> 8) + 128;

        r = rgba[4 * i];
        g = rgba[4 * i + 1];
        b = rgba[4 * i + 2];

        private_->rgba_to_yuv_buffer[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
      }
    }
    else
    {
      for (size_t x = 0; x < options.width; x += 1)
      {
        uint8_t r = rgba[4 * i];
        uint8_t g = rgba[4 * i + 1];
        uint8_t b = rgba[4 * i + 2];

        private_->rgba_to_yuv_buffer[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
      }
    }
  }

  add_frame_yuv(private_->rgba_to_yuv_buffer, yuv_size);
}
