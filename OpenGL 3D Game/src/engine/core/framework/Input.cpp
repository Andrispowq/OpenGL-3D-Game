#include "Input.h"

Input Input::instance;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto pushedKeys = Input::GetInstance().GetPushedKeys();
	auto keysHolding = Input::GetInstance().GetKeysHolding();
	auto releasedKeys = Input::GetInstance().GetReleasedKeys();

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

		auto index = std::find(pushedKeys.begin(), pushedKeys.end(), key);

		if (index != pushedKeys.end())
		{
			pushedKeys.erase(index);
			keysHolding.erase(index);
		}
	}

	Input::GetInstance().SetPushedKeys(pushedKeys);
	Input::GetInstance().SetKeysHolding(keysHolding);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input* const in = &(Input::GetInstance());

	if (button == 2 && action == GLFW_PRESS)
	{
		in->SetLockedCursorPosition(in->GetCursorPosition());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	if (button == 2 && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	auto pushedButtons = in->GetPushedButtons();
	auto buttonsHolding = in->GetButtonsHolding();
	auto releasedButtons = in->GetReleasedButtons();

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

		auto index = std::find(buttonsHolding.begin(), buttonsHolding.end(), button);
		if (index != buttonsHolding.end())
		{
			buttonsHolding.erase(index);
		}
	}

	in->SetPushedButtons(pushedButtons);
	in->SetButtonsHolding(buttonsHolding);
	in->SetReleasedButtons(releasedButtons);
}

void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
{
	Input::GetInstance().SetCursorPosition(Vector2f(static_cast<float>(xPos), static_cast<float>(yPos)));
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	Input::GetInstance().SetScrollOffset(static_cast<float>(yOffset));
}

void joystick_callback(int xPos, int yPos)
{
	Input::GetInstance().SetJoystickPosition(Vector2f(static_cast<float>(xPos), static_cast<float>(yPos)));
}

Input& Input::GetInstance()
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
	keysHolding.clear();

	pushedButtons.clear();
	buttonsHolding.clear();
	releasedButtons.clear();

	return true;
}