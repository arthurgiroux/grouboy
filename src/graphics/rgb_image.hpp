#ifndef GBEMULATOR_RGB_IMAGE_HPP
#define GBEMULATOR_RGB_IMAGE_HPP

#include "common/utils.hpp"
#include <vector>

/**
 *
 */
class RGBImage
{
  public:
    /**
     * Create a new RGB image with a given height / width
     *
     * @param height 	the height of the image
     * @param width 	the width of the image
     */
    RGBImage(int height, int width);

    ~RGBImage() = default;

    /**
     * Get the height of the image in pixels
     *
     * @return the height in pixels
     */
    int getHeight() const;

    /**
     * Get the width of the image in pixels
     *
     * @return the width in pixels
     */
    int getWidth() const;

    /**
     * Get the raw data representation of the image
     *
     * @return	an array of pixel
     */
    const std::vector<byte>& getData() const;

    /**
     * Set a given pixel to a specific color
     *
     * @param x 	the x coordinate in pixel
     * @param y 	the y coordinate in pixel
     * @param r 	the red component of the pixel [0;255]
     * @param g 	the green component of the pixel [0;255]
     * @param b 	the blue component of the pixel [0;255]
     */
    void setPixel(int x, int y, byte r, byte g, byte b);

    /**
     * Set a given pixel to a specific grayscale color
     *
     * @param x 		the x coordinate in pixel
     * @param y 		the y coordinate in pixel
     * @param value 	the grayscale value of the pixel [0;255]
     */
    void setPixel(int x, int y, byte value);

    /**
     * Copy a pixel from another image into this image.
     *
     * @param x 		the x coordinate where to copy the pixel
     * @param y 		the y coordinate where to copy the pixel
     * @param source 	the source image to use for the copy
     * @param sourceX 	the x coordinate to copy from
     * @param sourceY 	the y coordinate to copy from
     */
    void copyPixel(int x, int y, const RGBImage& source, int sourceX, int sourceY);

    /**
     * Get the red component of a given pixel
     *
     * @param x 	the x coordinate of the pixel
     * @param y 	the y coordinate of the pixel
     * @return 		the red component value [0;255]
     */
    byte getPixelR(int x, int y) const;

    /**
     * Get the green component of a given pixel
     *
     * @param x 	the x coordinate of the pixel
     * @param y 	the y coordinate of the pixel
     * @return 		the green component value [0;255]
     */
    byte getPixelG(int x, int y) const;

    /**
     * Get the blue component of a given pixel
     *
     * @param x 	the x coordinate of the pixel
     * @param y 	the y coordinate of the pixel
     * @return 		the blue component value [0;255]
     */
    byte getPixelB(int x, int y) const;

    /**
     * Return if the given pixel is white.
     *
     * @param x 	the x coordinate of the pixel
     * @param y 	the y coordinate of the pixel
     * @return 		true if pixel is white, false otherwise
     */
    bool isPixelWhite(int x, int y) const;

  private:
    int _height;
    int _width;
    static const int BYTES_PER_PIXEL = 3;
    std::vector<byte> _data = {};
};

#endif // GBEMULATOR_RGB_IMAGE_HPP
