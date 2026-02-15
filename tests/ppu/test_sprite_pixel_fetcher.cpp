#include "cpu/cpu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/pixel_fifo.hpp"
#include "graphics/ppu.hpp"
#include "graphics/sprite_pixel_fetcher.hpp"
#include <gtest/gtest.h>

class SpritePixelFetcherTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        mmu.setLcdStatusRegister(ppu.getLcdStatusRegister());
    }

    MMU mmu;
    CPU cpu = CPU(mmu);
    InterruptManager interruptManager = InterruptManager(&cpu);
    PPU ppu = PPU(mmu, &interruptManager);
    PixelFIFO oamFifo; // Changed from bgFifo - now we use a separate OAM FIFO

    // OAM addresses for sprite data
    static const word OAM_START = 0xFE00;
};

TEST_F(SpritePixelFetcherTest, FetcherShouldBeInactiveInitially)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);
    ASSERT_FALSE(fetcher.isActive());
}

TEST_F(SpritePixelFetcherTest, StartFetchingShouldActivateFetcher)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Create a sprite at position (16, 16) - which is (8, 0) on screen
    mmu.getOAM().write(0, 16);  // Y position
    mmu.getOAM().write(1, 16);  // X position
    mmu.getOAM().write(2, 0);   // Tile ID
    mmu.getOAM().write(3, 0);   // Attributes

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    ASSERT_TRUE(fetcher.isActive());
}

TEST_F(SpritePixelFetcherTest, ResetShouldDeactivateFetcher)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    mmu.getOAM().write(0, 16);
    mmu.getOAM().write(1, 16);
    mmu.getOAM().write(2, 0);
    mmu.getOAM().write(3, 0);

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);
    ASSERT_TRUE(fetcher.isActive());

    fetcher.reset();
    ASSERT_FALSE(fetcher.isActive());
}

TEST_F(SpritePixelFetcherTest, StepShouldReturnTrueWhenInactive)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);
    ASSERT_TRUE(fetcher.step());
}

TEST_F(SpritePixelFetcherTest, FetchShouldCompleteAfterCorrectNumberOfSteps)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Set up a sprite
    mmu.getOAM().write(0, 16);  // Y = 0 on screen
    mmu.getOAM().write(1, 16);  // X = 8 on screen
    mmu.getOAM().write(2, 0);   // Tile ID
    mmu.getOAM().write(3, 0);   // Attributes

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    // Sprite fetch takes 8 ticks:
    // 2 for GetTileId, 2 for GetTileDataLow, 2 for GetTileDataHigh, 2 for Sleep
    // Then Push is immediate
    int stepCount = 0;
    while (!fetcher.step())
    {
        stepCount++;
        ASSERT_LT(stepCount, 12); // Safety check to prevent infinite loop
    }

    // Should take 8 steps (2 per fetch phase including Sleep) before completing
    ASSERT_EQ(stepCount, 8);
    ASSERT_FALSE(fetcher.isActive());
}

TEST_F(SpritePixelFetcherTest, FetchShouldPushSpritePixelsToOAMFifo)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Set up VRAM with a tile that has some non-transparent pixels
    // Tile data starts at 0x8000, each tile is 16 bytes (2 bytes per row)
    // For simplicity, set first row to all color 3 (both bytes = 0xFF)
    mmu.getVRAM().write(0x0000, 0xFF); // Low byte
    mmu.getVRAM().write(0x0001, 0xFF); // High byte

    // Set up a sprite at X=8 (on screen X=0), Y=16 (on screen Y=0)
    mmu.getOAM().write(0, 16);  // Y
    mmu.getOAM().write(1, 8);   // X = 0 on screen
    mmu.getOAM().write(2, 0);   // Tile ID 0
    mmu.getOAM().write(3, 0);   // Attributes (no flip, palette 0, over BG)

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    // Run until complete
    while (!fetcher.step())
    {
    }

    // Check that OAM FIFO has 8 pixels (padded to 8 if needed, then filled)
    ASSERT_EQ(oamFifo.size(), 8);

    // Check that sprite pixels were pushed
    // Color 3 pixels (non-transparent) should be in the OAM FIFO
    for (int i = 0; i < 8; i++)
    {
        Pixel& pixel = oamFifo.at(i);
        ASSERT_EQ(pixel.getSource(), Pixel::Source::SPRITE);
        ASSERT_EQ(pixel.getColorId(), 3);
    }
}

TEST_F(SpritePixelFetcherTest, TransparentSpritePixelsShouldRemainTransparent)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Set up VRAM with a tile that has transparent pixels (color 0)
    // Both bytes = 0x00 means color 0 for all pixels
    mmu.getVRAM().write(0x0000, 0x00);
    mmu.getVRAM().write(0x0001, 0x00);

    // Set up sprite
    mmu.getOAM().write(0, 16);
    mmu.getOAM().write(1, 8);
    mmu.getOAM().write(2, 0);
    mmu.getOAM().write(3, 0);

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    while (!fetcher.step())
    {
    }

    // OAM FIFO should have transparent pixels (the sprite has color 0)
    // The FIFO is first padded with transparent pixels, then sprite data (also transparent) is merged
    ASSERT_EQ(oamFifo.size(), 8);
    for (int i = 0; i < 8; i++)
    {
        Pixel& pixel = oamFifo.at(i);
        ASSERT_EQ(pixel.getColorId(), 0); // Transparent
    }
}

TEST_F(SpritePixelFetcherTest, SpriteBehindBGShouldHaveNegativePriority)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Tile with all color 3
    mmu.getVRAM().write(0x0000, 0xFF);
    mmu.getVRAM().write(0x0001, 0xFF);

    // Sprite with BG priority flag set (bit 7 = 1)
    mmu.getOAM().write(0, 16);
    mmu.getOAM().write(1, 8);
    mmu.getOAM().write(2, 0);
    mmu.getOAM().write(3, 0x80); // BG over OBJ flag

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    while (!fetcher.step())
    {
    }

    // Sprite pixels should be in OAM FIFO with negative priority (indicates BG has priority)
    ASSERT_EQ(oamFifo.size(), 8);
    for (int i = 0; i < 8; i++)
    {
        Pixel& pixel = oamFifo.at(i);
        ASSERT_EQ(pixel.getSource(), Pixel::Source::SPRITE);
        ASSERT_EQ(pixel.getColorId(), 3);
        ASSERT_LT(pixel.getPriority(), 0); // Negative priority indicates BG has priority
    }
}

TEST_F(SpritePixelFetcherTest, SpriteShouldPushToOAMFifoRegardlessOfBGPriority)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Tile with all color 3
    mmu.getVRAM().write(0x0000, 0xFF);
    mmu.getVRAM().write(0x0001, 0xFF);

    // Sprite with BG priority flag set
    mmu.getOAM().write(0, 16);
    mmu.getOAM().write(1, 8);
    mmu.getOAM().write(2, 0);
    mmu.getOAM().write(3, 0x80);

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    while (!fetcher.step())
    {
    }

    // Sprite pixels should be in OAM FIFO (mixing with BG happens at render time)
    ASSERT_EQ(oamFifo.size(), 8);
    for (int i = 0; i < 8; i++)
    {
        Pixel& pixel = oamFifo.at(i);
        ASSERT_EQ(pixel.getSource(), Pixel::Source::SPRITE);
        ASSERT_EQ(pixel.getColorId(), 3);
    }
}

TEST_F(SpritePixelFetcherTest, HorizontallyFlippedSpriteShouldReversePixels)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Set up a tile where left half is color 1, right half is color 2
    // Bit pattern: 11110000 for low, 00001111 for high
    // This gives: pixels 0-3 = color 1, pixels 4-7 = color 2
    mmu.getVRAM().write(0x0000, 0xF0); // Low byte
    mmu.getVRAM().write(0x0001, 0x0F); // High byte

    // Sprite with horizontal flip
    mmu.getOAM().write(0, 16);
    mmu.getOAM().write(1, 8);
    mmu.getOAM().write(2, 0);
    mmu.getOAM().write(3, 0x20); // H-flip flag

    Sprite sprite(mmu.getOAM(), 0);
    fetcher.startFetching(&sprite, 0);

    while (!fetcher.step())
    {
    }

    // With H-flip, the order should be reversed
    // Original: pixels 0-3 = color 1, pixels 4-7 = color 2
    // Flipped: pixels 0-3 = color 2, pixels 4-7 = color 1
    ASSERT_EQ(oamFifo.size(), 8);
    for (int i = 0; i < 4; i++)
    {
        ASSERT_EQ(oamFifo.at(i).getColorId(), 2) << "Pixel " << i << " should be color 2";
    }
    for (int i = 4; i < 8; i++)
    {
        ASSERT_EQ(oamFifo.at(i).getColorId(), 1) << "Pixel " << i << " should be color 1";
    }
}

TEST_F(SpritePixelFetcherTest, HigherPriorityExistingSpriteShouldNotBeReplaced)
{
    SpritePixelFetcher fetcher(&mmu.getVRAM(), &ppu, oamFifo);

    // Tile with color 3
    mmu.getVRAM().write(0x0000, 0xFF);
    mmu.getVRAM().write(0x0001, 0xFF);

    // Sprite 1 (OAM index 1) - lower priority than sprite 0
    mmu.getOAM().write(4, 16);  // Y
    mmu.getOAM().write(5, 8);   // X
    mmu.getOAM().write(6, 0);   // Tile
    mmu.getOAM().write(7, 0);   // Flags

    // First, push existing sprite pixels with higher priority (lower OAM index = 0)
    for (int i = 0; i < 8; i++)
    {
        oamFifo.push(Pixel(2, 0, Pixel::Source::SPRITE, 0)); // OAM index 0 = higher priority
    }

    Sprite sprite(mmu.getOAM(), 1); // OAM index 1 = lower priority
    fetcher.startFetching(&sprite, 0);

    while (!fetcher.step())
    {
    }

    // Existing sprite pixels with higher priority (lower OAM index) should NOT be replaced
    ASSERT_EQ(oamFifo.size(), 8);
    for (int i = 0; i < 8; i++)
    {
        Pixel& pixel = oamFifo.at(i);
        ASSERT_EQ(pixel.getSource(), Pixel::Source::SPRITE);
        ASSERT_EQ(pixel.getColorId(), 2);
        ASSERT_EQ(pixel.getPriority(), 0); // Still has the original priority
    }
}
