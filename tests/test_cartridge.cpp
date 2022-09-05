#include "cartridge.hpp"
#include "utils.hpp"
#include <gtest/gtest.h>

class CartridgeTest : public ::testing::Test
{
  protected:
	void assertTitleIsReadCorrectly(const char* title) {
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