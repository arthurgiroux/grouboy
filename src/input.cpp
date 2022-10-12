#include "input.hpp"

Input::Input()
{
	buttonStates = {
	    {Button::UP, false}, {Button::DOWN, false}, {Button::LEFT, false},   {Button::RIGHT, false},
	    {Button::A, false},  {Button::B, false},    {Button::SELECT, false}, {Button::START, false},
	};
}

void Input::setButtonPressed(Input::Button button)
{
	buttonStates[button] = true;
}

void Input::setButtonReleased(Input::Button button)
{
	buttonStates[button] = false;
}

bool Input::isButtonPressed(Input::Button button) const
{
	return buttonStates.at(button);
}
