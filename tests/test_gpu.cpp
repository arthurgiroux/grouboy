#include <gtest/gtest.h>
#include "gpu.hpp"

class GpuTest : public ::testing::Test {
protected:
    GPU gpu;
};

TEST_F(GpuTest, DefaultModeShouldBeOam) {
    ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
}

TEST_F(GpuTest, DefaultScanlineShouldBeZero) {
    ASSERT_EQ(gpu.getCurrentScanline(), 0);
}

TEST_F(GpuTest, OamModeShouldntChangeIfNotEnoughTicksPassed) {
    gpu.step(GPU::OAM_ACCESS_TICKS - 1);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
}

TEST_F(GpuTest, ModeShouldSwitchFromOamToVramIfEnoughTicksPassed) {
    gpu.step(GPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
}

TEST_F(GpuTest, VramModeShouldntChangeIfNotEnoughTicksPassed) {
    gpu.step(GPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
    gpu.step(GPU::VRAM_ACCESS_TICKS - 1);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
}

TEST_F(GpuTest, ModeShouldSwitchFromVramToHblankIfEnoughTicksPassed) {
    gpu.step(GPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
    gpu.step(GPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
}

TEST_F(GpuTest, HBlankModeShouldntChangeIfNotEnoughTicksPassed) {
    gpu.step(GPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
    gpu.step(GPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
    gpu.step(GPU::HBLANK_TICKS - 1);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
}

TEST_F(GpuTest, ModeShouldSwitchFromHBlankToOAmIfEnoughTicksPassedAndItsNotTheLastScanline) {
    gpu.step(GPU::OAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
    gpu.step(GPU::VRAM_ACCESS_TICKS);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
    ASSERT_EQ(gpu.getCurrentScanline(), 0);
    gpu.step(GPU::HBLANK_TICKS);
    ASSERT_EQ(gpu.getCurrentScanline(), 1);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
}

TEST_F(GpuTest, ModeShouldSwitchFromHBlankToVBlankIfEnoughTicksPassedAndItsNotTheLastScanline) {
    for (int i = 0; i < GPU::SCREEN_HEIGHT; ++i) {
        gpu.step(GPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
        gpu.step(GPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
        ASSERT_EQ(gpu.getCurrentScanline(), i);
        gpu.step(GPU::HBLANK_TICKS);
        ASSERT_EQ(gpu.getCurrentScanline(), i + 1);

        if (i == GPU::SCREEN_HEIGHT - 1) {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::VBLANK);
        }
        else {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
        }
    }
}

TEST_F(GpuTest, VBlankModeShouldntChangeIfNotEnoughTicksPassed) {
    for (int i = 0; i < GPU::SCREEN_HEIGHT; ++i) {
        gpu.step(GPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
        gpu.step(GPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
        gpu.step(GPU::HBLANK_TICKS);

        if (i == GPU::SCREEN_HEIGHT - 1) {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::VBLANK);
        }
        else {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
        }
    }

    gpu.step(GPU::VBLANK_TICKS - 1);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::VBLANK);
}

TEST_F(GpuTest, ModeShouldSwitchFromVBlankToOamIfEnoughTicksPassedAndItsNotTheLastScanline) {
    for (int i = 0; i < GPU::SCREEN_HEIGHT; ++i) {
        gpu.step(GPU::OAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::VRAM_ACCESS);
        gpu.step(GPU::VRAM_ACCESS_TICKS);
        ASSERT_EQ(gpu.getMode(), GPU::Mode::HBLANK);
        ASSERT_EQ(gpu.getCurrentScanline(), i);
        gpu.step(GPU::HBLANK_TICKS);
        ASSERT_EQ(gpu.getCurrentScanline(), i + 1);

        if (i == GPU::SCREEN_HEIGHT - 1) {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::VBLANK);
        }
        else {
            ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
        }
    }

    gpu.step(GPU::VBLANK_TICKS);
    ASSERT_EQ(gpu.getCurrentScanline(), 0);
    ASSERT_EQ(gpu.getMode(), GPU::Mode::OAM_ACCESS);
}