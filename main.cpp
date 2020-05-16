// Copyright (C) Trevor Sundberg, MIT License (see LICENSE.md)
#include "h264_mp4_encoder.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
  printf("Starting encoding\n");
  h264_mp4_encoder encoder;
  encoder.set_width(1024);
  encoder.set_height(768);
  encoder.set_frame_rate(25);
  encoder.set_debug(false);

  int frame_size = 0;
  bool use_rgba = true;

  if (use_rgba) {
    frame_size = encoder.get_width() * encoder.get_height() * 4;
  } else {
    frame_size = encoder.get_width() * encoder.get_height() * 3 / 2;
  }

  std::string buffer;
  buffer.resize(frame_size);

  encoder.initialize();

  for (int i = 0; i < 100; ++i)
  {
    if (use_rgba) {
      gen_chessboard_rot_rgba((uint8_t*)buffer.data(), encoder.get_width(), encoder.get_height(), i);
      encoder.add_frame_rgba(buffer);
    } else {
      gen_chessboard_rot_yuv((uint8_t*)buffer.data(), encoder.get_width(), encoder.get_height(), i);
      encoder.add_frame_yuv(buffer);
    }
  }
  encoder.finalize();
  printf("Done encoding\n");
  return 0;
}