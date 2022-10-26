#ifndef GBEMULATOR_INPUT_CONTROLLER_HPP
#define GBEMULATOR_INPUT_CONTROLLER_HPP

#include <map>

/**
 * Controller for manual input through buttons
 */
class InputController
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
	InputController();

	/**
	 * Destroy an Input object
	 */
	~InputController() = default;

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

	/**
	 * Returns if a button is currently released
	 * @param button 	the button to check
	 * @return 	True is button is released, false otherwise
	 */
	bool isButtonReleased(Button button) const;

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

#endif // GBEMULATOR_INPUT_CONTROLLER_HPP
