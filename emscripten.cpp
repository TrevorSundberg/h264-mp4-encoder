#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "h264-mp4-encoder.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(H264MP4EncoderBinding)
{
  class_<H264MP4Encoder>("H264MP4Encoder")
      .constructor<>()

      .property("outputFilename", &H264MP4Encoder::get_outputFilename, &H264MP4Encoder::set_outputFilename)
      .property("width", &H264MP4Encoder::get_width, &H264MP4Encoder::set_width)
      .property("height", &H264MP4Encoder::get_height, &H264MP4Encoder::set_height)
      .property("frameRate", &H264MP4Encoder::get_frameRate, &H264MP4Encoder::set_frameRate)
      .property("kbps", &H264MP4Encoder::get_kbps, &H264MP4Encoder::set_kbps)
      .property("speed", &H264MP4Encoder::get_speed, &H264MP4Encoder::set_speed)
      .property("quantizationParameter", &H264MP4Encoder::get_quantizationParameter, &H264MP4Encoder::set_quantizationParameter)
      .property("groupOfPictures", &H264MP4Encoder::get_groupOfPictures, &H264MP4Encoder::set_groupOfPictures)
      .property("temporalDenoise", &H264MP4Encoder::get_temporalDenoise, &H264MP4Encoder::set_temporalDenoise)
      .property("desiredNaluBytes", &H264MP4Encoder::get_desiredNaluBytes, &H264MP4Encoder::set_desiredNaluBytes)
      .property("debug", &H264MP4Encoder::get_debug, &H264MP4Encoder::set_debug)

      .function("initialize", &H264MP4Encoder::initialize)
      .function("addFrameYuv", &H264MP4Encoder::addFrameYuv)
      .function("addFrameRgba", &H264MP4Encoder::addFrameRgba)
      .function("finalize", &H264MP4Encoder::finalize);
}