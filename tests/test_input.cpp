#include "input.hpp"
#include <gtest/gtest.h>

class InputTest : public ::testing::Test
{
  protected:
	void setAllButtonsPressed()
	{
		input.setButtonPressed(Input::Button::UP);
		input.setButtonPressed(Input::Button::DOWN);
		input.setButtonPressed(Input::Button::LEFT);
		input.setButtonPressed(Input::Button::RIGHT);
		input.setButtonPressed(Input::Button::A);
		input.setButtonPressed(Input::Button::B);
		input.setButtonPressed(Input::Button::SELECT);
		input.setButtonPressed(Input::Button::START);
	}

	Input input;
};

TEST_F(InputTest, AllButtonsAreReleasedByDefault)
{
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonUpCanBePressed)
{
	input.setButtonPressed(Input::Button::UP);
	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonUpCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));

	input.setButtonReleased(Input::Button::UP);

	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonDownCanBePressed)
{
	input.setButtonPressed(Input::Button::DOWN);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonDownCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));

	input.setButtonReleased(Input::Button::DOWN);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonLeftCanBePressed)
{
	input.setButtonPressed(Input::Button::LEFT);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonLeftCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));

	input.setButtonReleased(Input::Button::LEFT);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonRightCanBePressed)
{
	input.setButtonPressed(Input::Button::RIGHT);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonRightCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));

	input.setButtonReleased(Input::Button::RIGHT);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonACanBePressed)
{
	input.setButtonPressed(Input::Button::A);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonACanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));

	input.setButtonReleased(Input::Button::A);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonBCanBePressed)
{
	input.setButtonPressed(Input::Button::B);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonBCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));

	input.setButtonReleased(Input::Button::B);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonSelectCanBePressed)
{
	input.setButtonPressed(Input::Button::SELECT);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonSelectCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));

	input.setButtonReleased(Input::Button::SELECT);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonStartCanBePressed)
{
	input.setButtonPressed(Input::Button::START);
	ASSERT_FALSE(input.isButtonPressed(Input::Button::UP));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::A));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::B));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));
}

TEST_F(InputTest, ButtonStartCanBeReleased)
{
	setAllButtonsPressed();
	ASSERT_TRUE(input.isButtonPressed(Input::Button::START));

	input.setButtonReleased(Input::Button::START);

	ASSERT_TRUE(input.isButtonPressed(Input::Button::UP));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::DOWN));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::LEFT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::RIGHT));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::A));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::B));
	ASSERT_TRUE(input.isButtonPressed(Input::Button::SELECT));
	ASSERT_FALSE(input.isButtonPressed(Input::Button::START));
}