#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "h264_mp4_encoder.h"

using namespace emscripten;

static void emscripten_on_data_callback(
    void *userdata,
    const uint8_t *data,
    const uint32_t size) {
    emscripten::val& callback = *(emscripten::val*)userdata;
    callback(emscripten::val(emscripten::typed_memory_view(size, data)));
}

static emscripten::val get_on_data_callback(const h264_mp4_encoder& instance) {
  if (instance.on_data_callback_userdata) {
    return *(emscripten::val*)instance.on_data_callback_userdata;
  } else {
    return emscripten::val::null();
  }
}

static void set_on_data_callback(h264_mp4_encoder& instance, emscripten::val value) {
  instance.on_data_callback_userdata = new emscripten::val(value);
  instance.on_data_callback = &emscripten_on_data_callback;
}

EMSCRIPTEN_BINDINGS(h264_mp4_encoder_binding)
{
  class_<h264_mp4_encoder>("H264MP4Encoder")
      .constructor<>()

      .property("onDataCallback", &get_on_data_callback, &set_on_data_callback)

      .property("width", &h264_mp4_encoder::get_width, &h264_mp4_encoder::set_width)
      .property("height", &h264_mp4_encoder::get_height, &h264_mp4_encoder::set_height)
      .property("frameRate", &h264_mp4_encoder::get_frame_rate, &h264_mp4_encoder::set_frame_rate)
      .property("kbps", &h264_mp4_encoder::get_kbps, &h264_mp4_encoder::set_kbps)
      .property("speed", &h264_mp4_encoder::get_speed, &h264_mp4_encoder::set_speed)
      .property("quantizationParameter", &h264_mp4_encoder::get_quantization_parameter, &h264_mp4_encoder::set_quantization_parameter)
      .property("groupOfPictures", &h264_mp4_encoder::get_group_of_pictures, &h264_mp4_encoder::set_group_of_pictures)
      .property("temporalDenoise", &h264_mp4_encoder::get_temporal_denoise, &h264_mp4_encoder::set_temporal_denoise)
      .property("desiredNaluBytes", &h264_mp4_encoder::get_desired_nalu_bytes, &h264_mp4_encoder::set_desired_nalu_bytes)
      .property("debug", &h264_mp4_encoder::get_debug, &h264_mp4_encoder::set_debug)

      .function("initialize", &h264_mp4_encoder::initialize)
      .function("addFrameYuv", &h264_mp4_encoder::add_frame_yuv)
      .function("addFrameRgba", &h264_mp4_encoder::add_frame_rgba)
      .function("finalize", &h264_mp4_encoder::finalize);
}