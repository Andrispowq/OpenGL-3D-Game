#include "Input.h"

Input Input::instance;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
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

void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
{
	InputInstance.SetCursorPosition(Vector2f(static_cast<float>(xPos), static_cast<float>(yPos)));
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	InputInstance.SetScrollOffset(static_cast<float>(yOffset));
}

void joystick_callback(int xPos, int yPos)
{
	InputInstance.SetJoystickPosition(Vector2f(static_cast<float>(xPos), static_cast<float>(yPos)));
}

Input& InputInstance
{
	return instance;
}

bool Input::Init(const Window& window) const
{
	GLFWwindow* id = window.getWindow();

	glfwSetFramebufferSizeCallback(id, framebuffer_size_callback);
	glfwSetKeyCallback(id, key_callback);
	glfwSetMouseButtonCallback(id, mouse_callback);
	glfwSetCursorPosCallback(id, cursor_pos_callback);
	glfwSetScrollCallback(id, scroll_callback);
	glfwSetJoystickCallback(joystick_callback);

	return true;
}

bool Input::Update()
{
	scrollOffset = 0;

	pushedKeys.clear();
	releasedKeys.clear();

	pushedButtons.clear();
	releasedButtons.clear();

	return true;
}