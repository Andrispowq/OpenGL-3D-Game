#include "GLInput.h"

#include "GLWindow.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto pushedKeys = InputInstance.GetPushedKeys();
	auto keysHolding = InputInstance.GetKeysHolding();
	auto releasedKeys = InputInstance.GetReleasedKeys();

	if (action == GLFW_PRESS)
	{
		if (std::find(pushedKeys.begin(), pushedKeys.end(), key) == pushedKeys.end())
		{
			pushedKeys.push_back(key);
			keysHolding.push_back(key);
		}
	}

	if (action == GLFW_RELEASE)
	{
		releasedKeys.push_back(key);

		auto index0 = std::find(pushedKeys.begin(), pushedKeys.end(), key);
		auto index1 = std::find(keysHolding.begin(), keysHolding.end(), key);

		if (index0 != pushedKeys.end())
		{
			pushedKeys.erase(index0);
		}

		if (index1 != keysHolding.end())
		{
			keysHolding.erase(index1);
		}
	}

	InputInstance.SetPushedKeys(pushedKeys);
	InputInstance.SetKeysHolding(keysHolding);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == 2 && action == GLFW_PRESS)
	{
		InputInstance.SetLockedCursorPosition(InputInstance.GetCursorPosition());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	if (button == 2 && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	auto pushedButtons = InputInstance.GetPushedButtons();
	auto buttonsHolding = InputInstance.GetButtonsHolding();
	auto releasedButtons = InputInstance.GetReleasedButtons();

	if (action == GLFW_PRESS)
	{
		if (std::find(pushedButtons.begin(), pushedButtons.end(), button) == pushedButtons.end())
		{
			pushedButtons.push_back(button);
			buttonsHolding.push_back(button);
		}
	}

	if (action == GLFW_RELEASE)
	{
		releasedButtons.push_back(button);

		auto index0 = std::find(pushedButtons.begin(), pushedButtons.end(), button);
		auto index1 = std::find(buttonsHolding.begin(), buttonsHolding.end(), button);

		if (index0 != pushedButtons.end())
		{
			pushedButtons.erase(index0);
		}

		if (index1 != buttonsHolding.end())
		{
			buttonsHolding.erase(index1);
		}
	}

	InputInstance.SetPushedButtons(pushedButtons);
	InputInstance.SetButtonsHolding(buttonsHolding);
	InputInstance.SetReleasedButtons(releasedButtons);
}

static void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
{
	InputInstance.SetCursorPosition(Vector2f(static_cast<float>(xPos), static_cast<float>(yPos)));
}

static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	InputInstance.SetScrollOffset(static_cast<float>(yOffset));
}

bool GLInput::Init(Window* window) const
{
	GLWindow* glWind = reinterpret_cast<GLWindow*>(window);
	GLFWwindow* id = glWind->getWindow();

	glfwSetFramebufferSizeCallback(id, framebuffer_size_callback);
	glfwSetKeyCallback(id, key_callback);
	glfwSetMouseButtonCallback(id, mouse_callback);
	glfwSetCursorPosCallback(id, cursor_pos_callback);
	glfwSetScrollCallback(id, scroll_callback);

	return true;
}

bool GLInput::Update()
{
	scrollOffset = 0;

	pushedKeys.clear();
	releasedKeys.clear();

	pushedButtons.clear();
	releasedButtons.clear();

	//Set up joystick/gamepad
	for (unsigned int i = 0; i < MAX_NUM_JOYSTICKS; i++)
	{
		joystickButtons[i].clear();
		joystickAxes[i].clear();

		if (glfwJoystickPresent(GLFW_JOYSTICK_1 + i))
		{
			const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1 + i);

			if (name == NULL)
				printf("There is no joystick/gamepad number %i present!\n", i + 1);

			printf("%s\n", name);

			if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1 + i))
			{
				GLFWgamepadstate state;
				glfwGetGamepadState(GLFW_JOYSTICK_1 + i, &state);

				for (unsigned int j = 0; j < sizeof(state.buttons) / sizeof(state.buttons[0]); j++)
				{
					joystickButtons[i].push_back(state.buttons[j]);
				}

				for (unsigned int j = 0; j < sizeof(state.axes) / sizeof(state.axes[0]); j++)
				{
					joystickAxes[i].push_back(state.axes[j]);
				}
			}
			else
			{
				printf("Joystick %i is not a gamepad, and thus cannot be used by the game!\n", i + 1);
			}
		}
	}

	return true;
}

void GLInput::SetCursorPositionOnScreen(Window* window, const Vector2f& cursorPosition)
{
	glfwSetCursorPos(reinterpret_cast<GLWindow*>(window)->getWindow(), cursorPosition.x, cursorPosition.y);
}