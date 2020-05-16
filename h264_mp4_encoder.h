// Copyright (C) Trevor Sundberg, MIT License (see LICENSE.md)
#include <string>
#include <stdint.h>
#include <stdio.h>
#pragma once

class h264_mp4_encoder_private;

typedef void (*h264_mp4_encoder_data_callback)(
    void *userdata,
    const uint8_t *data,
    const uint32_t size);

#define HME_CHECK(expr, message)                                       \
  do                                                                   \
  {                                                                    \
    if (!(expr))                                                       \
    {                                                                  \
      printf("ERROR %s(%d): %s\n    EXPR: %s\n    FUNC: %s\n",         \
             __FILE__, __LINE__, message, #expr, __PRETTY_FUNCTION__); \
      abort();                                                         \
    }                                                                  \
  } while (false)

#define HME_CHECK_INTERNAL(expr) HME_CHECK(expr, "Internal error")

#define HME_PROPERTY(type, name, default_value)                     \
private:                                                            \
  type name = default_value;                                        \
                                                                    \
public:                                                             \
  const type &get_##name() const { return name; };                  \
  void set_##name(const type &value)                                \
  {                                                                 \
    HME_CHECK(!private_, "Cannot set properties after initialize"); \
    name = value;                                                   \
  };

class h264_mp4_encoder
{
public:
  friend class h264_mp4_encoder_private;

  HME_PROPERTY(std::string, output_filename, "output.mp4");

  HME_PROPERTY(uint32_t, width, 0);

  HME_PROPERTY(uint32_t, height, 0);

  HME_PROPERTY(uint32_t, frame_rate, 30);

  // Set the bitrate in kbps relative to the frame_rate. Overwrites quantization_parameter.
  HME_PROPERTY(uint32_t, kbps, 0);

  // Speed where 0 means best quality [0..10].
  HME_PROPERTY(uint32_t, speed, 0);

  // Higher means better compression, and lower means better quality [10..51].
  HME_PROPERTY(uint32_t, quantization_parameter, 33);

  // Key frame period.
  HME_PROPERTY(uint32_t, group_of_pictures, 20);

  // Use temporal noise supression.
  HME_PROPERTY(bool, temporal_denoise, false);

  // Each NAL unit will be approximately capped at this size (0 means unlimited).
  HME_PROPERTY(uint32_t, desired_nalu_bytes, 0);

  // Prints extra debug information.
  HME_PROPERTY(bool, debug, false);

  void initialize();

  void add_frame_yuv(const std::string &yuv_buffer);

  void add_frame_rgba(const std::string &rgba_buffer);

  void finalize();

  ~h264_mp4_encoder();

private:
  h264_mp4_encoder_private *private_ = nullptr;
};
