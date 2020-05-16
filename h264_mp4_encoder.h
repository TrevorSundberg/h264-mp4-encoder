// Copyright (C) Trevor Sundberg, MIT License (see LICENSE.md)
#include <stdint.h>
#pragma once

class h264_mp4_encoder_private;

typedef void (*h264_mp4_encoder_data_callback)(
    void *user_data,
    const uint8_t *data,
    const uint32_t size);

class h264_mp4_encoder_options
{
public:
  // Callback that is called every time we have data to write (required).
  h264_mp4_encoder_data_callback on_data_callback = nullptr;

  // Arbitrary user data to be passed to on_data_callback.
  void *on_data_callback_userdata = nullptr;

  uint32_t width = 0;

  uint32_t height = 0;

  uint32_t frame_rate = 30;

  // Set the bitrate in kbps relative to the frame_rate. Overwrites quantization_parameter.
  uint32_t kbps = 0;

  // Speed where 0 means best quality [0..10].
  uint32_t speed = 0;

  // Higher means better compression, and lower means better quality [10..51].
  uint32_t quantization_parameter = 33;

  // Key frame period.
  uint32_t group_of_pictures = 20;

  // Use temporal noise supression.
  bool temporal_denoise = false;

  // Each NAL unit will be approximately capped at this size (0 means unlimited).
  uint32_t desired_nalu_bytes = 0;

  // Prints extra debug information.
  bool debug = false;
};

class h264_mp4_encoder
{
public:
  h264_mp4_encoder(const h264_mp4_encoder_options &options);

  ~h264_mp4_encoder();

  void add_frame_yuv(uint8_t *yuv, const uint32_t size);

  void add_frame_rgba(const uint8_t *rgba, const uint32_t size);

  void finalize();

  const h264_mp4_encoder_options &options;

private:
  h264_mp4_encoder_private *const private_;
};
