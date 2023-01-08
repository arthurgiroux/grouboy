#include "cpu/cpu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/ppu.hpp"
#include <gtest/gtest.h>

class PpuTest : public ::testing::Test
{
  protected:
    MMU mmu;
    CPU cpu = CPU(mmu);
    InterruptManager interruptManager = InterruptManager(&cpu, &mmu);
    PPU ppu = PPU(mmu, &interruptManager);

    static const int ADDR_LCD_STATUS = 0xFF41;
    static const int ADDR_LYC = 0xFF45;
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

TEST_F(PpuTest, LCDStatModeFlagShouldBeSetToCorrectPPUMode)
{
    // We set LYC to a value outside of possible LY to avoid having to deal with it
    mmu.write(ADDR_LYC, 250);

    for (int scanlineId = 0; scanlineId < PPU::SCREEN_HEIGHT; ++scanlineId)
    {
        for (int i = 0; i < PPU::OAM_ACCESS_TICKS; ++i)
        {
            int expectedFlagMode = 0x02;
            ASSERT_EQ(mmu.read(ADDR_LCD_STATUS), expectedFlagMode);
            ASSERT_EQ(ppu.getMode(), PPU::Mode::OAM_ACCESS);
            ppu.step(1);
        }

        for (int i = 0; i < PPU::VRAM_ACCESS_TICKS; ++i)
        {
            int expectedFlagMode = 0x03;
            ASSERT_EQ(mmu.read(ADDR_LCD_STATUS), expectedFlagMode);
            ASSERT_EQ(ppu.getMode(), PPU::Mode::VRAM_ACCESS);
            ppu.step(1);
        }

        for (int i = 0; i < PPU::HBLANK_TICKS; ++i)
        {
            int expectedFlagMode = 0x00;
            ASSERT_EQ(mmu.read(ADDR_LCD_STATUS), expectedFlagMode);
            ASSERT_EQ(ppu.getMode(), PPU::Mode::HBLANK);
            ppu.step(1);
        }
    }

    for (int scanlineId = PPU::SCREEN_HEIGHT; scanlineId < PPU::MAX_SCANLINE_VALUE; ++scanlineId)
    {
        for (int i = 0; i < PPU::VBLANK_TICKS; ++i)
        {
            int expectedFlagMode = 0x01;
            ASSERT_EQ(mmu.read(ADDR_LCD_STATUS), expectedFlagMode);
            ASSERT_EQ(ppu.getMode(), PPU::Mode::VBLANK);
            ppu.step(1);
        }
    }
}

TEST_F(PpuTest, LCDStatLYCCompareShouldOnlyBeSetWhenLYIsTheSameAsLCY)
{
    int expectedLine = 25;
    mmu.write(ADDR_LYC, expectedLine);

    for (int scanlineId = 0; scanlineId < PPU::SCREEN_HEIGHT; ++scanlineId)
    {
        ppu.step(1);
        ASSERT_EQ(utils::isNthBitSet(mmu.read(ADDR_LCD_STATUS), 2), scanlineId == expectedLine);
        ppu.step(PPU::OAM_ACCESS_TICKS - 1);
        ppu.step(PPU::VRAM_ACCESS_TICKS);
        ppu.step(PPU::HBLANK_TICKS);
        ppu.step(PPU::HBLANK_TICKS);
    }

    // Let's check that it also works during VBLANK
    expectedLine = 155;
    mmu.write(ADDR_LYC, expectedLine);

    for (int scanlineId = PPU::SCREEN_HEIGHT; scanlineId < PPU::MAX_SCANLINE_VALUE; ++scanlineId)
    {
        ppu.step(1);
        ASSERT_EQ(utils::isNthBitSet(mmu.read(ADDR_LCD_STATUS), 2), scanlineId == expectedLine);
        ppu.step(PPU::VBLANK_TICKS - 1);
    }
}

TEST_F(PpuTest, LCDStatLYCCompareShouldRaiseInterruptWhenLYIsTheSameAsLCY)
{
    int expectedLine = 25;
    mmu.write(ADDR_LYC, expectedLine);
    mmu.write(ADDR_LCD_STATUS, 0x40);

    ASSERT_FALSE(interruptManager.isInterruptPending(InterruptType::LCD_STAT));

    for (int scanlineId = 0; scanlineId < PPU::SCREEN_HEIGHT; ++scanlineId)
    {
        ppu.step(1);
        ASSERT_EQ(interruptManager.isInterruptPending(InterruptType::LCD_STAT), scanlineId == expectedLine);
        ppu.step(PPU::OAM_ACCESS_TICKS - 1);
        ppu.step(PPU::VRAM_ACCESS_TICKS);
        ppu.step(PPU::HBLANK_TICKS);
        interruptManager.clearInterrupt(InterruptType::LCD_STAT);
    }
}