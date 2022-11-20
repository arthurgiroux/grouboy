#include "graphics/ppu.hpp"
#include <gtest/gtest.h>

class PpuTest : public ::testing::Test
{
  protected:
    MMU mmu;
    PPU ppu = PPU(mmu);
};

TEST_F(PpuTest, DefaultModeShouldBeOam)
{
    ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
}

TEST_F(PpuTest, DefaultScanlineShouldBeZero)
{
    ASSERT_EQ(ppu.getCurrentScanline(), 0);
}

TEST_F(PpuTest, OamModeShouldntChangeIfNotEnoughTicksPassed)
{
    ppu.step(PPU::OAM_ACCESS_TICKS - 1);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
}

TEST_F(PpuTest, ModeShouldSwitchFromOamToVramIfEnoughTicksPassed)
{
    ppu.step(PPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
}

TEST_F(PpuTest, VramModeShouldntChangeIfNotEnoughTicksPassed)
{
    ppu.step(PPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
    ppu.step(PPU::VRAM_ACCESS_TICKS - 1);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
}

TEST_F(PpuTest, ModeShouldSwitchFromVramToHblankIfEnoughTicksPassed)
{
    ppu.step(PPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
    ppu.step(PPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
}

TEST_F(PpuTest, HBlankModeShouldntChangeIfNotEnoughTicksPassed)
{
    ppu.step(PPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
    ppu.step(PPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
    ppu.step(PPU::HBLANK_TICKS - 1);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
}

TEST_F(PpuTest, ModeShouldSwitchFromHBlankToOAmIfEnoughTicksPassedAndItsNotTheLastScanline)
{
    ppu.step(PPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
    ppu.step(PPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
    ASSERT_EQ(ppu.getCurrentScanline(), 0);
    ppu.step(PPU::HBLANK_TICKS);
    ASSERT_EQ(ppu.getCurrentScanline(), 1);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
}

TEST_F(PpuTest, ModeShouldSwitchFromHBlankToVBlankIfEnoughTicksPassedAndItsNotTheLastScanline)
{
    for (int i = 0; i < PPU::SCREEN_HEIGHT; ++i)
    {
        ppu.step(PPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
        ppu.step(PPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
        ASSERT_EQ(ppu.getCurrentScanline(), i);
        ppu.step(PPU::HBLANK_TICKS);
        ASSERT_EQ(ppu.getCurrentScanline(), i + 1);

        if (i == PPU::SCREEN_HEIGHT - 1)
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::VBLANK);
        }
        else
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
        }
    }
}

TEST_F(PpuTest, VBlankModeShouldntChangeIfNotEnoughTicksPassed)
{
    for (int i = 0; i < PPU::SCREEN_HEIGHT; ++i)
    {
        ppu.step(PPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
        ppu.step(PPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
        ppu.step(PPU::HBLANK_TICKS);

        if (i == PPU::SCREEN_HEIGHT - 1)
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::VBLANK);
        }
        else
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
        }
    }

    ppu.step(PPU::VBLANK_TICKS - 1);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::VBLANK);
}

TEST_F(PpuTest, ModeShouldSwitchFromVBlankToOamIfEnoughTicksPassedAndItsNotTheLastScanline)
{
    for (int i = 0; i < PPU::SCREEN_HEIGHT; ++i)
    {
        ppu.step(PPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
        ppu.step(PPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
        ASSERT_EQ(ppu.getCurrentScanline(), i);
        ppu.step(PPU::HBLANK_TICKS);
        ASSERT_EQ(ppu.getCurrentScanline(), i + 1);

        if (i == PPU::SCREEN_HEIGHT - 1)
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::VBLANK);
        }
        else
        {
            ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
        }
    }

    for (int i = PPU::SCREEN_HEIGHT + 1; i <= PPU::MAX_SCANLINE_VALUE; ++i)
    {
        ppu.step(PPU::VBLANK_TICKS);
        ASSERT_EQ(ppu.getCurrentScanline(), i);
    }

    ppu.step(PPU::VBLANK_TICKS);
    ASSERT_EQ(ppu.getCurrentScanline(), 0);
    ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
}