#include "bitmap/bitmap_image.hpp"
#include "emulator.hpp"
#include <gtest/gtest.h>

class AcidTest : public ::testing::Test
{
  protected:
    void assertROMOutputIsSimilarToReferenceImage(std::string rom, std::string refImagePath)
    {
        Emulator emulator;

        bitmap_image referenceImage(refImagePath);

        ASSERT_FALSE(!referenceImage);

        ASSERT_EQ(referenceImage.width(), PPU::SCREEN_WIDTH);
        ASSERT_EQ(referenceImage.height(), PPU::SCREEN_HEIGHT);

        ASSERT_TRUE(emulator.getMMU().loadCartridgeFromFile(rom));

        // We wait a certain number of frames to make sure we are seeing the result of the acid test
        int expectedFrameId = 500;
        while (emulator.getPPU().getFrameId() < expectedFrameId)
        {
            emulator.exec();
        }

        const RGBImage& image = emulator.getPPU().getLastRenderedFrame();

        for (int y = 0; y < PPU::SCREEN_HEIGHT; ++y)
        {
            for (int x = 0; x < PPU::SCREEN_WIDTH; ++x)
            {
                rgb_t referenceColor;
                referenceImage.get_pixel(x, y, referenceColor);

                ASSERT_EQ(image.getPixelR(x, y), referenceColor.red);
                ASSERT_EQ(image.getPixelG(x, y), referenceColor.green);
                ASSERT_EQ(image.getPixelB(x, y), referenceColor.blue);
            }
        }
    }
};

TEST_F(AcidTest, DMGAcidShouldPass)
{
    std::string rom = std::string(DATADIR) + "/roms/acid/dmg-acid2.gb";
    std::string reImage = std::string(DATADIR) + "/reference/acid-reference-dmg.bmp";
    assertROMOutputIsSimilarToReferenceImage(rom, reImage);
}

TEST_F(AcidTest, CGBAcidShouldPass)
{
    std::string rom = std::string(DATADIR) + "/roms/acid/cgb-acid2.gbc";
    std::string reImage = std::string(DATADIR) + "/reference/acid-reference-cgb.bmp";
    assertROMOutputIsSimilarToReferenceImage(rom, reImage);
}