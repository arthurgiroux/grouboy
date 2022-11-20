#include "cpu/input_controller.hpp"
#include <gtest/gtest.h>

class InputControllerTest : public ::testing::Test
{
  protected:
    void setAllButtonsPressed()
    {
        inputController.setButtonPressed(InputController::Button::UP);
        inputController.setButtonPressed(InputController::Button::DOWN);
        inputController.setButtonPressed(InputController::Button::LEFT);
        inputController.setButtonPressed(InputController::Button::RIGHT);
        inputController.setButtonPressed(InputController::Button::A);
        inputController.setButtonPressed(InputController::Button::B);
        inputController.setButtonPressed(InputController::Button::SELECT);
        inputController.setButtonPressed(InputController::Button::START);
    }

    InputController inputController;
};

TEST_F(InputControllerTest, AllButtonsAreReleasedByDefault)
{
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonUpCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::UP);
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonUpCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));

    inputController.setButtonReleased(InputController::Button::UP);

    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonDownCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::DOWN);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonDownCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));

    inputController.setButtonReleased(InputController::Button::DOWN);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonLeftCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::LEFT);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonLeftCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));

    inputController.setButtonReleased(InputController::Button::LEFT);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonRightCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::RIGHT);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonRightCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));

    inputController.setButtonReleased(InputController::Button::RIGHT);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonACanBePressed)
{
    inputController.setButtonPressed(InputController::Button::A);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonACanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));

    inputController.setButtonReleased(InputController::Button::A);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonBCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::B);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonBCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));

    inputController.setButtonReleased(InputController::Button::B);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonSelectCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::SELECT);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonSelectCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));

    inputController.setButtonReleased(InputController::Button::SELECT);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonStartCanBePressed)
{
    inputController.setButtonPressed(InputController::Button::START);
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));
}

TEST_F(InputControllerTest, ButtonStartCanBeReleased)
{
    setAllButtonsPressed();
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::START));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::START));

    inputController.setButtonReleased(InputController::Button::START);

    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::UP));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::DOWN));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::LEFT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::RIGHT));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::A));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::B));
    ASSERT_TRUE(inputController.isButtonPressed(InputController::Button::SELECT));
    ASSERT_FALSE(inputController.isButtonPressed(InputController::Button::START));

    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::UP));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::DOWN));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::LEFT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::RIGHT));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::A));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::B));
    ASSERT_FALSE(inputController.isButtonReleased(InputController::Button::SELECT));
    ASSERT_TRUE(inputController.isButtonReleased(InputController::Button::START));
}