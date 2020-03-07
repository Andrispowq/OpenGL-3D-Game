#include "Input.h"

#include "engine/platform/windows/opengl/framework/GLInput.h"
#include <algorithm>

Input* Input::instance = nullptr;

Input& Input::GetInstance()
{
	if (instance == nullptr)
	{
		if (FrameworkConfig::api == OpenGL)
		{
			instance = new GLInput();
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			printf("Vulkan API is not yet supported!");
			exit(-1);
		}
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