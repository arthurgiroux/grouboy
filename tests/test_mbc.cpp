#include "memory/mbc/mbc_rom_only.hpp"
#include "memory/mbc/memory_bank_controller.hpp"
#include <gtest/gtest.h>

class MBCROMOnlyTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::vector<byte> data{};
        data.resize(romSize);
        int cartridgeTypeAddr = 0x0147;
        data[cartridgeTypeAddr] = Cartridge::CartridgeType::ROM_ONLY;
        int cartridgeSizeValue = 0x0148;
        // Value 0 corresponds to 32KiB
        data[cartridgeSizeValue] = 0;
        cartridge = std::make_unique<Cartridge>(data);
        mbc = std::make_unique<MBCRomOnly>(cartridge.get());
    }

    int romSize = 32_KiB;
    std::unique_ptr<Cartridge> cartridge = nullptr;
    std::unique_ptr<MBCRomOnly> mbc = nullptr;
};

TEST_F(MBCROMOnlyTest, ReadROMInBoundsShouldReturnValue)
{
    word addr = 0x1234;
    int value = 0x42;
    cartridge->getData()[addr] = value;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBCROMOnlyTest, ReadROMOutsideOfBoundsShouldThrowException)
{
    word addr = 0xFFFF;
    ASSERT_THROW(mbc->readROM(addr), std::runtime_error);
}

TEST_F(MBCROMOnlyTest, WriteROMShouldThrowException)
{
    ASSERT_THROW(mbc->writeROM(0x00, 0x00), std::runtime_error);
}

TEST_F(MBCROMOnlyTest, ReadRAMShouldThrowException)
{
    ASSERT_THROW(mbc->readRAM(0x00), std::runtime_error);
}

TEST_F(MBCROMOnlyTest, WriteRAMShouldThrowException)
{
    ASSERT_THROW(mbc->writeRAM(0x00, 0x00), std::runtime_error);
}