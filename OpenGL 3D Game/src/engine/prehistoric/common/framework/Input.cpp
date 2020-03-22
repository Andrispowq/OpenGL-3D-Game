#include "engine/prehistoric/core/util/Includes.hpp"
#include "Input.h"
#include "engine/platform/Prehistoric.h"
#include "engine/platform/windows/WindowsInput.h"

Input* Input::instance = nullptr;

Input& Input::GetInstance()
{
	if (instance == nullptr)
	{
#if defined(PR_WINDOWS_64)
		instance = new WindowsInput();
#endif
	}

	return *instance;
}

void Input::DeleteInstance()
{
	delete instance;
}

bool Input::IsJoystickButtonPushed(const int& key, const int& joystick) const
{
	auto buttons = joystickButtons[joystick];

	if (std::find(buttons.begin(), buttons.end(), key) != buttons.end())
	{
		return buttons[key] == 1;
	}

	return false;
}

float Input::GetJoystickAxisOffset(const int& axis, const int& joystick) const
{
	auto axes = joystickAxes[joystick];

	if (std::find(axes.begin(), axes.end(), axis) != axes.end())
	{
		return axes[axis];
	}

	return 0;
}