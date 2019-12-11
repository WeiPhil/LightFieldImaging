#include <cmath>
#include <string>

#include "qulkan/logger.h"
#include "utils/pngwriter.h"
#include <iostream>

PNGWriter::PNGWriter(int width, int height, int channels, float *imageBuffer) : width(width), height(height), imageBuffer(imageBuffer), channels(channels) {

    // this->imageBuffer = createMandelbrotImage(width, height, -0.802f, -0.177f, 0.011f, 110);
}

int PNGWriter::writePNG(const char *filename, const char *imageTitle) {
    png_byte color_type = channels == 4 ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;
    png_byte bit_depth = 8;

    png_structp png_ptr;
    png_infop info_ptr;

    /* create file */
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        Qulkan::Logger::Error("[write_png_file] File %s could not be opened for writing", filename);

    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        Qulkan::Logger::Error("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        Qulkan::Logger::Error("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        Qulkan::Logger::Error("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        Qulkan::Logger::Error("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        Qulkan::Logger::Error("[write_png_file] Error during writing bytes");

    // png_write_image(png_ptr, rows);
    png_byte **rows = new png_byte *[height];
    for (int i = 0; i < height; i++)
        rows[i] = new png_byte[width * channels];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                rows[y][x * channels + c] = imageBuffer[(((height - 1) - y) * width + x) * channels + c] * 255.0f;
            }
        }
    }

    png_write_image(png_ptr, rows);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        Qulkan::Logger::Error("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    Qulkan::Logger::Info("PNGWriter: Wrote png file to %s \n", filename);

    fclose(fp);

    return 0;
}