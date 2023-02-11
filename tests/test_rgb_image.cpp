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

TEST(RGBImage, FillShouldSetAllPixelsToTheGivenValue)
{
    RGBImage image(480, 250);
    byte colorValue = 128;
    image.fill(colorValue);
    for (int y = 0; y < image.getHeight(); ++y)
    {
        for (int x = 0; x < image.getWidth(); ++x)
        {
            ASSERT_EQ(image.getPixelR(x, y), colorValue);
            ASSERT_EQ(image.getPixelG(x, y), colorValue);
            ASSERT_EQ(image.getPixelB(x, y), colorValue);
        }
    }
}

TEST(RGBImage, CopyRegionShouldCopySelectedRegionInTargetImage)
{
    RGBImage image(480, 250);
    int offsetX = 12;
    int offsetY = 52;
    RGBImage sourceImage(50, 30);
    int defaultColor = 20;
    image.fill(defaultColor);

    int sourceColor = 42;
    sourceImage.fill(sourceColor);

    int regionOffsetX = 2;
    int regionOffsetY = 3;

    int regionHeight = 25;
    int regionWidth = 12;

    image.copyRegion(offsetX, offsetY, sourceImage, regionOffsetX, regionOffsetY, regionHeight, regionWidth);

    for (int y = 0; y < image.getHeight(); ++y)
    {
        for (int x = 0; x < image.getWidth(); ++x)
        {
            int expectedColor = defaultColor;
            if ((x >= offsetX && x < offsetX + regionWidth) &&
                (y >= offsetY && y < offsetY + regionHeight)) {
                    expectedColor = sourceColor;
            }

            ASSERT_EQ(image.getPixelR(x, y), expectedColor);
            ASSERT_EQ(image.getPixelG(x, y), expectedColor);
            ASSERT_EQ(image.getPixelB(x, y), expectedColor);
        }
    }
}

TEST(RGBImage, CopyImageShouldCopyWholeImageInTargetImage)
{
    RGBImage image(480, 250);
    int offsetX = 12;
    int offsetY = 52;
    RGBImage sourceImage(50, 30);
    int defaultColor = 20;
    image.fill(defaultColor);

    int sourceColor = 42;
    sourceImage.fill(sourceColor);

    image.copyImage(offsetX, offsetY, sourceImage);

    for (int y = 0; y < image.getHeight(); ++y)
    {
        for (int x = 0; x < image.getWidth(); ++x)
        {
            int expectedColor = defaultColor;
            if ((x >= offsetX && x < offsetX + sourceImage.getWidth()) &&
                (y >= offsetY && y < offsetY + sourceImage.getHeight())) {
                expectedColor = sourceColor;
            }

            ASSERT_EQ(image.getPixelR(x, y), expectedColor);
            ASSERT_EQ(image.getPixelG(x, y), expectedColor);
            ASSERT_EQ(image.getPixelB(x, y), expectedColor);
        }
    }
}