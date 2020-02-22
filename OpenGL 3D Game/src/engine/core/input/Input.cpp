#include "Input.h"

Input Input::instance;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void invoke_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto pushedKeys = Input::GetInstance().GetPushedKeys();

	if (action == GLFW_PRESS)
	{
		if (std::find(pushedKeys.begin(), pushedKeys.end(), key) == pushedKeys.end())
		{
			pushedKeys.push_back(key);
			pushedKeys.push_back(key);
		}
	}

	if (action == GLFW_RELEASE)
	{
		auto index = std::find(pushedKeys.begin(), pushedKeys.end(), key);
		if (index != pushedKeys.end())
		{
			pushedKeys.erase(index);
			pushedKeys.erase(index);
		}
	}

	Input::GetInstance().SetPushedKeys(pushedKeys);
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