#include "common/utils.hpp"
#include "memory/cartridge.hpp"
#include <gtest/gtest.h>

class CartridgeTest : public ::testing::Test
{
  protected:
    void assertTitleIsReadCorrectly(const char* title)
    {
        std::vector<byte> data;
        data.resize(0x200);
        std::copy(title, title + strlen(title), data.data() + 0x0134);

        auto cartridge = Cartridge(data);
        auto cartridgeTitle = cartridge.getTitle();

        ASSERT_EQ(cartridgeTitle.length(), strlen(title));
        for (int i = 0; i < cartridgeTitle.length(); ++i)
        {
            EXPECT_EQ(cartridgeTitle[i], title[i]);
        }
    }

    void assertCartridgeTypeIsReadSuccessfully(int cartridgeTypeValue, Cartridge::CartridgeType expectedValue)
    {
        std::vector<byte> data;
        data.resize(0x200);
        int typeAddr = 0x147;
        data[typeAddr] = cartridgeTypeValue;

        auto cartridge = Cartridge(data);
        int cartridgeType = cartridge.getType();

        ASSERT_EQ(cartridgeType, expectedValue);
    }

    void assertROMSizeValueGivesExpectedResult(int romSizeValue, int expectedROMSize)
    {
        std::vector<byte> data;
        data.resize(0x200);
        int romSizeAddr = 0x148;
        data[romSizeAddr] = romSizeValue;

        auto cartridge = Cartridge(data);
        int romSize = cartridge.getROMSize();

        ASSERT_EQ(romSize, expectedROMSize);
    }

    void assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType cartridgeType, int ramSizeValue,
                                               int expectedRAMSize)
    {
        std::vector<byte> data;
        data.resize(0x200);
        int ramSizeAddr = 0x149;
        data[ramSizeAddr] = ramSizeValue;
        int typeAddr = 0x147;
        data[typeAddr] = static_cast<int>(cartridgeType);

        auto cartridge = Cartridge(data);
        int ramSize = cartridge.getRAMSize();

        ASSERT_EQ(ramSize, expectedRAMSize);
    }

    void assertColorModeGivesExpectedResult(int colorFlag, bool expectedColorModeSupport)
    {
        std::vector<byte> data;
        data.resize(0x200);
        int colorModeAddr = 0x143;
        data[colorModeAddr] = colorFlag;

        auto cartridge = Cartridge(data);

        ASSERT_EQ(cartridge.isColorModeSupported(), expectedColorModeSupport);
    }
};

TEST(Cartridge, CartridgeDataIsLoadedCorrectly)
{
    std::vector<byte> data{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    auto cartridge = Cartridge(data);
    auto cartridgeData = cartridge.getData();
    ASSERT_EQ(cartridgeData.size(), data.size());
    for (int i = 0; i < cartridgeData.size(); ++i)
    {
        EXPECT_EQ(cartridgeData[i], data[i]) << "Data differs at index " << i;
    }
}

TEST_F(CartridgeTest, CartridgeShortTitleIsReadCorrectly)
{
    assertTitleIsReadCorrectly("TEST");
}

TEST_F(CartridgeTest, CartridgeLongTitleIsReadCorrectly)
{
    assertTitleIsReadCorrectly("TEST TITLE!");
}

TEST(Cartridge, CartridgeManufacturerCodeIsReadCorrectly)
{
    std::vector<byte> data;
    data.resize(0x200);
    const char* code = "AFGI";
    std::copy(code, code + strlen(code), data.data() + 0x013F);

    auto cartridge = Cartridge(data);
    auto cartridgeCode = cartridge.getManufacturerCode();

    ASSERT_EQ(cartridgeCode.length(), strlen(code));
    for (int i = 0; i < cartridgeCode.length(); ++i)
    {
        EXPECT_EQ(cartridgeCode[i], code[i]);
    }
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn32KiBForValue0)
{
    assertROMSizeValueGivesExpectedResult(0x00, 32_KiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn64KiBForValue1)
{
    assertROMSizeValueGivesExpectedResult(0x01, 64_KiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn128KiBForValue2)
{
    assertROMSizeValueGivesExpectedResult(0x02, 128_KiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn256KiBForValue3)
{
    assertROMSizeValueGivesExpectedResult(0x03, 256_KiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn512KiBForValue4)
{
    assertROMSizeValueGivesExpectedResult(0x04, 512_KiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn1MiBForValue5)
{
    assertROMSizeValueGivesExpectedResult(0x05, 1_MiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn2MiBForValue6)
{
    assertROMSizeValueGivesExpectedResult(0x06, 2_MiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn4MiBForValue7)
{
    assertROMSizeValueGivesExpectedResult(0x07, 4_MiB);
}

TEST_F(CartridgeTest, ROMSizeValueShouldReturn8MiBForValue8)
{
    assertROMSizeValueGivesExpectedResult(0x08, 8_MiB);
}

TEST_F(CartridgeTest, RAMSizeValueShouldReturn0WhenTheresNoRAM)
{
    assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType::MBC1, 2, 0_KiB);
}

TEST_F(CartridgeTest, RAMSizeValueShouldReturn8KiBWhenTheresRAMAndValueIs2)
{
    assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType::MBC1_RAM, 2, 8_KiB);
}

TEST_F(CartridgeTest, RAMSizeValueShouldReturn32KiBWhenTheresRAMAndValueIs3)
{
    assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType::MBC1_RAM, 3, 32_KiB);
}

TEST_F(CartridgeTest, RAMSizeValueShouldReturn128KiBWhenTheresRAMAndValueIs4)
{
    assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType::MBC1_RAM, 4, 128_KiB);
}

TEST_F(CartridgeTest, RAMSizeValueShouldReturn64KiBWhenTheresRAMAndValueIs5)
{
    assertRAMSizeValueGivesExpectedResult(Cartridge::CartridgeType::MBC1_RAM, 5, 64_KiB);
}

TEST_F(CartridgeTest, CartridgeTypeShouldBeROMOnlyForValue0)
{
    assertCartridgeTypeIsReadSuccessfully(0x00, Cartridge::CartridgeType::ROM_ONLY);
}

TEST_F(CartridgeTest, CartridgeTypeShouldBeMMM01ForValue11)
{
    assertCartridgeTypeIsReadSuccessfully(0x0B, Cartridge::CartridgeType::MMM01);
}

TEST_F(CartridgeTest, CartridgeTypeShouldBeMBC6ForValue32)
{
    assertCartridgeTypeIsReadSuccessfully(0x20, Cartridge::CartridgeType::MBC6);
}

TEST_F(CartridgeTest, IsColorModeSupportedShouldReturnTrueWhenFlagisColorOnly)
{
    assertColorModeGivesExpectedResult(0xC0, true);
}

TEST_F(CartridgeTest, IsColorModeSupportedShouldReturnTrueWhenFlagisColorAndMono)
{
    assertColorModeGivesExpectedResult(0x80, true);
}

TEST_F(CartridgeTest, IsColorModeSupportedShouldReturnFalseWhenFlagisMono)
{
    assertColorModeGivesExpectedResult(0x00, false);
}