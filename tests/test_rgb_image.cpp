#include "graphics/rgb_image.hpp"
#include <gtest/gtest.h>

TEST(RGBImage, ConstructorSetsHeightAndWidthCorrectly)
{
    int expectedHeight = 250;
    int expectedWidth = 480;

    RGBImage image(expectedHeight, expectedWidth);
    ASSERT_EQ(image.getHeight(), expectedHeight);
    ASSERT_EQ(image.getWidth(), expectedWidth);
    ASSERT_EQ(image.getData().size(), expectedHeight * expectedWidth * 3);
}

TEST(RGBImage, SetPixelWithGrayscaleValueChangesRGBValueToGivenValue)
{
    RGBImage image(480, 250);
    int x = 128;
    int y = 300;
    int value = 42;
    image.setPixel(x, y, value);
    ASSERT_EQ(image.getPixelR(x, y), value);
    ASSERT_EQ(image.getPixelG(x, y), value);
    ASSERT_EQ(image.getPixelB(x, y), value);
}

TEST(RGBImage, SetPixelWithRGBValueChangesRGBValueToGivenValue)
{
    RGBImage image(480, 250);
    int x = 128;
    int y = 300;
    int valueR = 42;
    int valueG = 43;
    int valueB = 44;
    image.setPixel(x, y, valueR, valueG, valueB);
    ASSERT_EQ(image.getPixelR(x, y), valueR);
    ASSERT_EQ(image.getPixelG(x, y), valueG);
    ASSERT_EQ(image.getPixelB(x, y), valueB);
}

TEST(RGBImage, CopyPixelFromOtherPixelShouldSetPixelFromSource)
{
    RGBImage targetImage(480, 250);
    RGBImage sourceImage(480, 250);

    int x = 128;
    int y = 300;
    int targetX = 129;
    int targetY = 301;

    int valueR = 42;
    int valueG = 43;
    int valueB = 44;
    sourceImage.setPixel(x, y, valueR, valueG, valueB);
    targetImage.copyPixel(targetX, targetY, sourceImage, x, y);

    ASSERT_EQ(targetImage.getPixelR(targetX, targetY), valueR);
    ASSERT_EQ(targetImage.getPixelG(targetX, targetY), valueG);
    ASSERT_EQ(targetImage.getPixelB(targetX, targetY), valueB);
}

TEST(RGBImage, IsPixelWhiteShouldReturnTrueForWhitePixel)
{
    RGBImage image(480, 250);
    int x = 128;
    int y = 300;
    int value = 255;
    image.setPixel(x, y, value);
    ASSERT_TRUE(image.isPixelWhite(x, y));
}

TEST(RGBImage, IsPixelWhiteShouldReturnFalseForBlackPixel)
{
    RGBImage image(480, 250);
    int x = 128;
    int y = 300;
    int value = 0;
    image.setPixel(x, y, value);
    ASSERT_FALSE(image.isPixelWhite(x, y));
}