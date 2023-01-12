#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include "common/types.hpp"
#include "rgb_image.hpp"
#include <array>

class Tile
{
  public:
    static const int BYTES_PER_TILE_VALUE = 2;

    using TileDataArray = std::vector<byte>;
    using ColorDataArray = std::vector<byte>;

    Tile(const TileDataArray& data, int height, int width);

    const RGBImage& getImage() const;
    byte getColorData(int x, int y) const;

    int getHeight() const;
    int getWidth() const;

  protected:
    void convertToPixels();

    TileDataArray _data = {};
    ColorDataArray _colorData = {};
    int _height;
    int _width;
    RGBImage _image;
};

class SingleTile : public Tile
{
  public:
    static const int BYTES_PER_TILE = 16;
    static const int TILE_WIDTH = 8;
    static const int TILE_HEIGHT = 8;

    SingleTile(const TileDataArray& data);
};

class StackedTile : public Tile
{
  public:
    static const int BYTES_PER_TILE = 32;
    static const int TILE_WIDTH = 8;
    static const int TILE_HEIGHT = 16;

    StackedTile(const TileDataArray& data);
};

#endif // GBEMULATOR_TILE_H
