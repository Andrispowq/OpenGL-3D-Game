#include "Input.h"

Input Input::instance;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void invoke_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (std::find(Input::GetInstance().GetPushedKeys().begin(), Input::GetInstance().GetPushedKeys().end(), key) == Input::GetInstance().GetPushedKeys().end())
		{
			Input::GetInstance().GetPushedKeys().push_back(key);
			Input::GetInstance().GetKeysHolding().push_back(key);
		}
	}

	if (action == GLFW_RELEASE)
	{
		auto index = std::find(Input::GetInstance().GetPushedKeys().begin(), Input::GetInstance().GetPushedKeys().end(), key);
		Input::GetInstance().GetPushedKeys().erase(index);
		Input::GetInstance().GetKeysHolding().erase(index);
	}
}

Input& Input::GetInstance()
{
	return instance;
}

bool Input::Init(const Window& window) const
{
	glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	glfwSetKeyCallback(window.getWindow(), invoke_callback);

	return true;
}

bool Input::Update()
{
	scrollOffset = 0;

	pushedKeys.clear();
	keysHolding.clear();
	pushedButtons.clear();
	buttonsHolding.clear();

	return true;
}