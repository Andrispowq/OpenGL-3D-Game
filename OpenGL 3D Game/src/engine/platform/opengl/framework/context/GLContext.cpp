#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLContext.h"

bool GLContext::InitContext(Window* window)
{
	glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(window->GetWindowHandle()));

	GLenum error = glewInit();

	glewExperimental = GL_FALSE;

	if (error != GLEW_OK)
	{
		PR_LOG_ERROR("The initialisation of OpenGL Extension Wrangler Library (GLEW) has failed! The error \n%s\n", glewGetErrorString(error));

		glfwTerminate();
		return false;
	}

	return true;
}

bool GLContext::DeleteContext(Window* window)
{
	return true;
}