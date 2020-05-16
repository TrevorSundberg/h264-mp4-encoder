// Copyright (C) Trevor Sundberg, MIT License (see LICENSE.md)
#include "h264_mp4_encoder.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static int pixel_of_chessboard(double x, double y)
{
  int mid = (fabs(x) < 4 && fabs(y) < 4);
  int i = (int)(x);
  int j = (int)(y);
  int black = (mid) ? 128 : i / 16;
  int white = (mid) ? 128 : 255 - j / 16;
  int c00 = (((i >> 4) + (j >> 4)) & 1) ? white : black;
  int c01 = ((((i + 1) >> 4) + (j >> 4)) & 1) ? white : black;
  int c10 = (((i >> 4) + ((j + 1) >> 4)) & 1) ? white : black;
  int c11 = ((((i + 1) >> 4) + ((j + 1) >> 4)) & 1) ? white : black;
  int s = (int)((c00 * (1 - (x - i)) + c01 * (x - i)) * (1 - (y - j)) +
                (c10 * (1 - (x - i)) + c11 * (x - i)) * ((y - j)) + 0.5);
  return s < 0 ? 0 : s > 255 ? 255 : s;
}

static void gen_chessboard_rot_yuv(unsigned char *p, int w, int h, int frm)
{
  memset(p + w * h, 128, w *h / 2);
  double co = cos(.01 * frm);
  double si = sin(.01 * frm);
  int hw = w >> 1;
  int hh = h >> 1;
  for (int r = 0; r < h; r++)
  {
    for (int c = 0; c < w; c++)
    {
      double x = co * (c - hw) + si * (r - hh);
      double y = -si * (c - hw) + co * (r - hh);
      p[r * w + c] = pixel_of_chessboard(x, y);
    }
  }
}

static void gen_chessboard_rot_rgba(unsigned char *p, int w, int h, int frm)
{
  memset(p + w * h, 128, w *h / 2);
  double co = cos(.01 * frm);
  double si = sin(.01 * frm);
  int hw = w >> 1;
  int hh = h >> 1;
  for (int r = 0; r < h; r++)
  {
    for (int c = 0; c < w; c++)
    {
      double x = co * (c - hw) + si * (r - hh);
      double y = -si * (c - hw) + co * (r - hh);
      int base = (r * w + c) * 4;
      int color = pixel_of_chessboard(x / 2, y / 2);
      p[base + 0] = color;
      p[base + 1] = color / 2;
      p[base + 2] = color;
      p[base + 3] = 255;
    }
  }
}

static void on_data_callback(
    void *user_data,
    const uint8_t *data,
    const uint32_t size)
{
  printf("GOT DATA: %d\n", (int)size);
}

int main(int argc, char *argv[])
{
  h264_mp4_encoder_options options;
  options.on_data_callback = &on_data_callback;
  options.width = 1024;
  options.height = 768;
  options.frame_rate = 25;
  options.debug = false;

  h264_mp4_encoder encoder(options);

  int frame_size = 0;
  bool use_rgba = true;
  uint8_t *buffer = nullptr;

  if (use_rgba) {
    frame_size = options.width * options.height * 4;
    buffer = (uint8_t *)malloc(frame_size);
  } else {
    frame_size = options.width * options.height * 3 / 2;
    uint8_t *buffer = (uint8_t *)malloc(frame_size);
  }

  for (int i = 0; i < 100; ++i)
  {
    if (use_rgba) {
      gen_chessboard_rot_rgba(buffer, options.width, options.height, i);
      encoder.add_frame_rgba(buffer, frame_size);
    } else {
      gen_chessboard_rot_yuv(buffer, options.width, options.height, i);
      encoder.add_frame_yuv(buffer, frame_size);
    }
  }

  return 0;
}