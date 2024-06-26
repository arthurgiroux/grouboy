#include "graphics/pixel_fifo.hpp"
#include <gtest/gtest.h>

class PixelFIFOTest : public ::testing::Test
{
  protected:
    PixelFIFO fifo;
};

TEST_F(PixelFIFOTest, PixelFIFOShouldBeEmptyAtInit)
{
    ASSERT_EQ(fifo.size(), 0);
    ASSERT_TRUE(fifo.isEmpty());
    ASSERT_FALSE(fifo.isFull());
}

TEST_F(PixelFIFOTest, PushShouldIncreaseFIFO)
{
    auto pixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
    fifo.push(pixel);
    ASSERT_EQ(fifo.size(), 1);
}

TEST_F(PixelFIFOTest, PixelFIFOWithOneElementShouldNotBeEmpty)
{
    auto pixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
    fifo.push(pixel);
    ASSERT_FALSE(fifo.isEmpty());
}

TEST_F(PixelFIFOTest, PopShouldRemoveFirstElement)
{
    auto firstPixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
    auto secondPixel = Pixel(0xF0, nullptr, Pixel::Source::SPRITE);
    fifo.push(firstPixel);
    fifo.push(secondPixel);
    ASSERT_EQ(fifo.pop(), firstPixel);
    ASSERT_EQ(fifo.pop(), secondPixel);
}

TEST_F(PixelFIFOTest, PixelFIFOWith16ElementsShouldBeFull)
{
    ASSERT_FALSE(fifo.isFull());
    int maxSize = 16;
    for (int i = 0; i < maxSize; ++i)
    {
        auto pixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
        fifo.push(pixel);
        ASSERT_EQ(fifo.size(), i + 1);
    }
    ASSERT_TRUE(fifo.isFull());
}

TEST_F(PixelFIFOTest, ClearShouldRemoveAllElements)
{
    int maxSize = 16;
    for (int i = 0; i < maxSize; ++i)
    {
        auto pixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
        fifo.push(pixel);
    }
    ASSERT_TRUE(fifo.isFull());
    fifo.clear();
    ASSERT_FALSE(fifo.isFull());
    ASSERT_TRUE(fifo.isEmpty());
}
