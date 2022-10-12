#ifndef GBEMULATOR_INPUT_HPP
#define GBEMULATOR_INPUT_HPP

#include <map>

class Input
{
  public:
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

	Input();
	~Input() = default;

	void setButtonPressed(Button button);
	void setButtonReleased(Button button);
	bool isButtonPressed(Button button) const;

  private:
	std::map<Button, bool> buttonStates = {};
};

#endif // GBEMULATOR_INPUT_HPP
