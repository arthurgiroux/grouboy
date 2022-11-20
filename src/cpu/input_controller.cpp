#include "input_controller.hpp"

InputController::InputController()
{
	buttonStates = {
	    {Button::UP, ButtonState::RELEASED},     {Button::DOWN, ButtonState::RELEASED},
	    {Button::LEFT, ButtonState::RELEASED},   {Button::RIGHT, ButtonState::RELEASED},
	    {Button::A, ButtonState::RELEASED},      {Button::B, ButtonState::RELEASED},
	    {Button::SELECT, ButtonState::RELEASED}, {Button::START, ButtonState::RELEASED},
	};
}

void InputController::setButtonPressed(InputController::Button button)
{
	buttonStates[button] = ButtonState::PRESSED;
}

void InputController::setButtonReleased(InputController::Button button)
{
	buttonStates[button] = ButtonState::RELEASED;
}

bool InputController::isButtonPressed(InputController::Button button) const
{
	return buttonStates.at(button) == ButtonState::PRESSED;
}

bool InputController::isButtonReleased(InputController::Button button) const
{
	return buttonStates.at(button) == ButtonState::RELEASED;
}
