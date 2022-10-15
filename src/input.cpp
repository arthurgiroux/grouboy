#include "input.hpp"

Input::Input()
{
	buttonStates = {
	    {Button::UP, ButtonState::RELEASED},     {Button::DOWN, ButtonState::RELEASED},
	    {Button::LEFT, ButtonState::RELEASED},   {Button::RIGHT, ButtonState::RELEASED},
	    {Button::A, ButtonState::RELEASED},      {Button::B, ButtonState::RELEASED},
	    {Button::SELECT, ButtonState::RELEASED}, {Button::START, ButtonState::RELEASED},
	};
}

void Input::setButtonPressed(Input::Button button)
{
	buttonStates[button] = ButtonState::PRESSED;
}

void Input::setButtonReleased(Input::Button button)
{
	buttonStates[button] = ButtonState::RELEASED;
}

bool Input::isButtonPressed(Input::Button button) const
{
	return buttonStates.at(button) == ButtonState::PRESSED;
}
