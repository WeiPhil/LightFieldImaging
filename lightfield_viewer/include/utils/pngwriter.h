#pragma once

#ifndef PNGWRITER_H
#define PNGWRITER_H

#include <png.h>

class PNGWriter {

  private:
    int width, height, channels;
    float *imageBuffer;

  public:
    PNGWriter(int width, int height, int channels, float *imageBuffer);
    ~PNGWriter() { delete imageBuffer; };

    int writePNG(const char *filename, const char *imageTitle);
};

#endif
