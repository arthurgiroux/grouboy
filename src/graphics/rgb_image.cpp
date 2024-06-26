#include "rgb_image.hpp"
#include <cassert>

RGBImage::RGBImage(int height, int width) : _height(height), _width(width)
{
    _data.resize(height * width * BYTES_PER_PIXEL);
}

int RGBImage::getHeight() const
{
    return _height;
}

int RGBImage::getWidth() const
{
    return _width;
}

const std::vector<byte>& RGBImage::getData() const
{
    return _data;
}

void RGBImage::setPixel(int x, int y, byte r, byte g, byte b)
{
    assert(x >= 0 && x < _width);
    assert(y >= 0 && y < _height);

    _data[(y * _width + x) * BYTES_PER_PIXEL + 0] = r;
    _data[(y * _width + x) * BYTES_PER_PIXEL + 1] = g;
    _data[(y * _width + x) * BYTES_PER_PIXEL + 2] = b;
}

void RGBImage::setPixel(int x, int y, byte value)
{
    setPixel(x, y, value, value, value);
}

void RGBImage::copyPixel(int x, int y, const RGBImage& source, int sourceX, int sourceY)
{
    byte r = source.getPixelR(sourceX, sourceY);
    byte g = source.getPixelG(sourceX, sourceY);
    byte b = source.getPixelB(sourceX, sourceY);
    setPixel(x, y, r, g, b);
}

byte RGBImage::getPixelR(int x, int y) const
{
    return _data[(y * _width + x) * BYTES_PER_PIXEL + 0];
}

byte RGBImage::getPixelG(int x, int y) const
{
    return _data[(y * _width + x) * BYTES_PER_PIXEL + 1];
}

byte RGBImage::getPixelB(int x, int y) const
{
    return _data[(y * _width + x) * BYTES_PER_PIXEL + 2];
}

bool RGBImage::isPixelWhite(int x, int y) const
{
    int r = getPixelR(x, y);
    int g = getPixelG(x, y);
    int b = getPixelB(x, y);

    return r == 255 && g == 255 && b == 255;
}

void RGBImage::fill(byte value)
{
    std::fill(_data.begin(), _data.end(), value);
}

void RGBImage::copyRegion(int x, int y, const RGBImage& source, int sourceX, int sourceY, int height, int width)
{
    for (int offsetY = 0; (offsetY < height) && ((y + offsetY) < _height); ++offsetY)
    {
        for (int offsetX = 0; (offsetX < width) && ((x + offsetX) < _width); ++offsetX)
        {
            copyPixel(x + offsetX, y + offsetY, source, sourceX + offsetX, sourceY + offsetY);
        }
    }
}

void RGBImage::copyImage(int x, int y, const RGBImage& source)
{
    copyRegion(x, y, source, 0, 0, source.getHeight(), source.getWidth());
}

void RGBImage::setPixel(int x, int y, RGBColor color)
{
    setPixel(x, y, color.getRed(), color.getGreen(), color.getBlue());
}
