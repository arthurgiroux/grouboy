#include "memory/mbc/mbc1.hpp"
#include "memory/mbc/mbc2.hpp"
#include "memory/mbc/mbc3.hpp"
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
        int cartridgeSizeAddr = 0x0148;
        // Value 0 corresponds to 32KiB
        int cartridgeSizeValue = 0;
        data[cartridgeSizeAddr] = cartridgeSizeValue;
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

TEST_F(MBCROMOnlyTest, SerializeUnserializedRAMShouldLoadStateCorrectly)
{
    auto serialized = mbc->serializeRAM();
    // No RAM to serialize
    ASSERT_EQ(serialized.size(), 0);
    ASSERT_TRUE(mbc->unserializeRAM(serialized));
}

class MBC1Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::vector<byte> data{};
        data.resize(romSize);
        for (int i = 0; i < romSize; ++i)
        {
            // We set value of the ROM to its bank rom id
            data[i] = (i / romBankSize);
        }

        int cartridgeTypeAddr = 0x0147;
        data[cartridgeTypeAddr] = Cartridge::CartridgeType::MBC1_RAM;
        int cartridgeSizeAddr = 0x0148;
        // Value 2 corresponds to 128KiB
        int cartridgeSizeValue = 2;
        data[cartridgeSizeAddr] = cartridgeSizeValue;

        int cartridgeRamSizeAddr = 0x0149;
        // Value 3 corresponds to 32KiB
        int cartridgeRamSizeValue = 3;
        data[cartridgeRamSizeAddr] = cartridgeRamSizeValue;
        cartridge = std::make_unique<Cartridge>(data);
        mbc = std::make_unique<MBC1>(cartridge.get());
    }

    void switchROMBank(int bankId)
    {
        mbc->writeROM(0x2000, bankId);
    }

    void switchRAMBank(int bankId)
    {
        mbc->writeROM(0x4000, bankId);
    }

    void enableRam()
    {
        mbc->writeROM(0x0000, 0x0A);
    }

    void disableRam()
    {
        mbc->writeROM(0x0000, 0x00);
    }

    int romSize = 128_KiB;
    int romBankSize = 16_KiB;
    std::unique_ptr<Cartridge> cartridge = nullptr;
    std::unique_ptr<MBC1> mbc = nullptr;
};

TEST_F(MBC1Test, ReadROMInFixedBankShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, ReadROMInSwappableBankShouldReturnBank1ByDefault)
{
    word addr = 0x4001;
    int value = 0x01;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, ReadROMInSwappableBankAfterSwappedToBank2ShouldReturnBank2)
{
    word addr = 0x4001;
    int value = 0x02;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, ReadROMInFixedBankAfterSwappedToBank2ShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, ReadROMInSwappableBankAfterSwappedToBank0ShouldReturnBank1)
{
    word addr = 0x4001;
    int value = 0x01;
    switchROMBank(0);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, ReadRAMWhenRAMIsDisabledShouldReturnFF)
{
    word addr = 0x0001;
    int value = 0xFF;
    ASSERT_EQ(mbc->readRAM(addr), value);
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), 0x00);
    disableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC1Test, ReadRAMWhenRAMIsEnabledShouldReturnValue)
{
    word addr = 0x0001;
    int value = 0x00;
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC1Test, WriteRAMWhenRAMIsDisabledShouldNotSetValue)
{
    word addr = 0x0001;
    int value = 0xFF;
    mbc->writeRAM(addr, 0x00);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC1Test, WriteRAMWhenRAMIsEnabledShouldSetValue)
{
    word addr = 0x0001;
    int value = 0x01;
    enableRam();
    mbc->writeRAM(addr, value);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC1Test, SwappingRAMBankShouldRetainBankValue)
{
    word addr = 0x0001;
    int valueBank0 = 0x00;
    int valueBank1 = 0x01;
    enableRam();
    mbc->writeRAM(addr, valueBank0);
    ASSERT_EQ(mbc->readRAM(addr), valueBank0);
    switchRAMBank(1);
    mbc->writeRAM(addr, valueBank1);
    ASSERT_EQ(mbc->readRAM(addr), valueBank1);
    switchRAMBank(0);
    ASSERT_EQ(mbc->readRAM(addr), valueBank0);
}

TEST_F(MBC1Test, ForROMSmallerThan256KiBSwitchingBankOutsideOfPossibleValueShouldMapToBank0)
{
    word addr = 0x4001;
    int value = 0x00;
    switchROMBank(0b00010000);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC1Test, SerializeUnserializedRAMShouldLoadStateCorrectly)
{
    // Set the RAM to a determined state
    enableRam();
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i));
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i + 1);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i + 1));
    }

    // Serialize current state
    auto serialized = mbc->serializeRAM();

    // Reset RAM
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, 0);
        ASSERT_EQ(mbc->readRAM(i), 0);
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, 0);
        ASSERT_EQ(mbc->readRAM(i), 0);
    }

    // Load serialize state
    ASSERT_TRUE(mbc->unserializeRAM(serialized));

    // Check that state was correctly restored
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i));
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i + 1);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i + 1));
    }
}

class MBC2Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::vector<byte> data{};
        data.resize(romSize);
        for (int i = 0; i < romSize; ++i)
        {
            // We set value of the ROM to its bank rom id
            data[i] = (i / romBankSize);
        }

        int cartridgeTypeAddr = 0x0147;
        data[cartridgeTypeAddr] = Cartridge::CartridgeType::MBC2;
        int cartridgeSizeAddr = 0x0148;
        // Value 2 corresponds to 128KiB
        int cartridgeSizeValue = 2;
        data[cartridgeSizeAddr] = cartridgeSizeValue;

        cartridge = std::make_unique<Cartridge>(data);
        mbc = std::make_unique<MBC2>(cartridge.get());
    }

    void switchROMBank(int bankId)
    {
        mbc->writeROM(0x100, bankId);
    }

    void enableRam()
    {
        mbc->writeROM(0x0000, 0x0A);
    }

    void disableRam()
    {
        mbc->writeROM(0x0000, 0x00);
    }

    int romSize = 128_KiB;
    int romBankSize = 16_KiB;
    std::unique_ptr<Cartridge> cartridge = nullptr;
    std::unique_ptr<MBC2> mbc = nullptr;
};

TEST_F(MBC2Test, ReadROMInFixedBankShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC2Test, ReadROMInSwappableBankShouldReturnBank1ByDefault)
{
    word addr = 0x4001;
    int value = 0x01;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC2Test, ReadROMInSwappableBankAfterSwappedToBank2ShouldReturnBank2)
{
    word addr = 0x4001;
    int value = 0x02;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC2Test, ReadROMInFixedBankAfterSwappedToBank2ShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC2Test, ReadROMInSwappableBankAfterSwappedToBank0ShouldReturnBank1)
{
    word addr = 0x4001;
    int value = 0x01;
    switchROMBank(0);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC2Test, ReadRAMWhenRAMIsDisabledShouldReturnFF)
{
    word addr = 0x0001;
    int value = 0xFF;
    ASSERT_EQ(mbc->readRAM(addr), value);
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), 0x00);
    disableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC2Test, ReadRAMWhenRAMIsEnabledShouldReturnValue)
{
    word addr = 0x0001;
    int value = 0x00;
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC2Test, WriteRAMWhenRAMIsDisabledShouldNotSetValue)
{
    word addr = 0x0001;
    int value = 0xFF;
    mbc->writeRAM(addr, 0x00);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC2Test, WriteRAMWhenRAMIsEnabledShouldSetValue)
{
    word addr = 0x0001;
    int value = 0x01;
    enableRam();
    mbc->writeRAM(addr, value);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC2Test, WriteRAMShouldOnlyWriteHalfByteValue)
{
    word addr = 0x0001;
    int value = 0xFF;
    int expectedValue = 0x0F;
    enableRam();
    mbc->writeRAM(addr, value);
    ASSERT_EQ(mbc->readRAM(addr), expectedValue);
}

TEST_F(MBC2Test, SerializeUnserializedRAMShouldLoadStateCorrectly)
{
    // Set the RAM to a determined state
    enableRam();
    for (int i = 0; i < 512_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i & 0xF));
    }

    // Serialize current state
    auto serialized = mbc->serializeRAM();

    // Reset RAM
    for (int i = 0; i < 512_KiB; ++i)
    {
        mbc->writeRAM(i, 0);
        ASSERT_EQ(mbc->readRAM(i), 0);
    }

    // Load serialize state
    ASSERT_TRUE(mbc->unserializeRAM(serialized));

    // Check that state was correctly restored
    for (int i = 0; i < 512_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i & 0xF));
    }
}

class MBC3Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::vector<byte> data{};
        data.resize(romSize);
        for (int i = 0; i < romSize; ++i)
        {
            // We set value of the ROM to its bank rom id
            data[i] = (i / romBankSize);
        }

        int cartridgeTypeAddr = 0x0147;
        data[cartridgeTypeAddr] = Cartridge::CartridgeType::MBC3_TIMER_RAM_BATTERY_2;
        int cartridgeSizeAddr = 0x0148;
        // Value 2 corresponds to 128KiB
        int cartridgeSizeValue = 2;
        data[cartridgeSizeAddr] = cartridgeSizeValue;

        int cartridgeRamSizeAddr = 0x0149;
        // Value 3 corresponds to 32KiB
        int cartridgeRamSizeValue = 3;
        data[cartridgeRamSizeAddr] = cartridgeRamSizeValue;
        cartridge = std::make_unique<Cartridge>(data);
        mbc = std::make_unique<MBC3>(cartridge.get());
    }

    void switchROMBank(int bankId)
    {
        mbc->writeROM(0x2000, bankId);
    }

    void switchRAMBank(int bankId)
    {
        mbc->writeROM(0x4000, bankId);
    }

    void enableRam()
    {
        mbc->writeROM(0x0000, 0x0A);
    }

    void disableRam()
    {
        mbc->writeROM(0x0000, 0x00);
    }

    int romSize = 128_KiB;
    int romBankSize = 16_KiB;
    std::unique_ptr<Cartridge> cartridge = nullptr;
    std::unique_ptr<MBC3> mbc = nullptr;
};

TEST_F(MBC3Test, ReadROMInFixedBankShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC3Test, ReadROMInSwappableBankShouldReturnBank1ByDefault)
{
    word addr = 0x4001;
    int value = 0x01;
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC3Test, ReadROMInSwappableBankAfterSwappedToBank2ShouldReturnBank2)
{
    word addr = 0x4001;
    int value = 0x02;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC3Test, ReadROMInFixedBankAfterSwappedToBank2ShouldReturnBank0)
{
    word addr = 0x0001;
    int value = 0x00;
    switchROMBank(2);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC3Test, ReadROMInSwappableBankAfterSwappedToBank0ShouldReturnBank1)
{
    word addr = 0x4001;
    int value = 0x01;
    switchROMBank(0);
    ASSERT_EQ(mbc->readROM(addr), value);
}

TEST_F(MBC3Test, ReadRAMWhenRAMIsDisabledShouldReturnFF)
{
    word addr = 0x0001;
    int value = 0xFF;
    ASSERT_EQ(mbc->readRAM(addr), value);
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), 0x00);
    disableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC3Test, ReadRAMWhenRAMIsEnabledShouldReturnValue)
{
    word addr = 0x0001;
    int value = 0x00;
    enableRam();
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC3Test, WriteRAMWhenRAMIsDisabledShouldNotSetValue)
{
    word addr = 0x0001;
    int value = 0xFF;
    mbc->writeRAM(addr, 0x00);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC3Test, WriteRAMWhenRAMIsEnabledShouldSetValue)
{
    word addr = 0x0001;
    int value = 0x01;
    enableRam();
    mbc->writeRAM(addr, value);
    ASSERT_EQ(mbc->readRAM(addr), value);
}

TEST_F(MBC3Test, SwappingRAMBankShouldRetainBankValue)
{
    word addr = 0x0001;
    int valueBank0 = 0x00;
    int valueBank1 = 0x01;
    enableRam();
    mbc->writeRAM(addr, valueBank0);
    ASSERT_EQ(mbc->readRAM(addr), valueBank0);
    switchRAMBank(1);
    mbc->writeRAM(addr, valueBank1);
    ASSERT_EQ(mbc->readRAM(addr), valueBank1);
    switchRAMBank(0);
    ASSERT_EQ(mbc->readRAM(addr), valueBank0);
}

TEST_F(MBC3Test, SerializeUnserializedRAMShouldLoadStateCorrectly)
{
    // Set the RAM to a determined state
    enableRam();
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i));
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i + 1);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i + 1));
    }

    // Serialize current state
    auto serialized = mbc->serializeRAM();

    // Reset RAM
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, 0);
        ASSERT_EQ(mbc->readRAM(i), 0);
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, 0);
        ASSERT_EQ(mbc->readRAM(i), 0);
    }

    // Load serialize state
    ASSERT_TRUE(mbc->unserializeRAM(serialized));

    // Check that state was correctly restored
    switchRAMBank(0);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i));
    }
    switchRAMBank(1);
    for (int i = 0; i < 8_KiB; ++i)
    {
        mbc->writeRAM(i, i + 1);
        ASSERT_EQ(mbc->readRAM(i), static_cast<byte>(i + 1));
    }
}