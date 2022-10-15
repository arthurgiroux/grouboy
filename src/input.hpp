#ifndef GBEMULATOR_INPUT_HPP
#define GBEMULATOR_INPUT_HPP

#include <map>

/**
 * Abstraction class for manual input through buttons
 */
class Input
{
  public:
	/**
	 * Names of buttons that can be pressed
	 */
	enum class Button
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A,
		B,
		SELECT,
		START,
	};

	/**
	 * Create a new Input object
	 */
	Input();

	/**
	 * Destroy an Input object
	 */
	~Input() = default;

	/**
	 * Set the given button as being pressed
	 * @param button 	the button that was pressed
	 */
	void setButtonPressed(Button button);

	/**
	 * Set the given button as being released
	 * @param button	the button that was released
	 */
	void setButtonReleased(Button button);

	/**
	 * Returns if a button is currently pressed
	 * @param button 	the button to check
	 * @return 	True is button is pressed, false otherwise
	 */
	bool isButtonPressed(Button button) const;

  private:
	/**
	 * Possible state for a button
	 */
	enum class ButtonState
	{
		PRESSED,
		RELEASED
	};

	/**
	 * Current state of all buttons
	 */
	std::map<Button, ButtonState> buttonStates = {};
};

#endif // GBEMULATOR_INPUT_HPP
