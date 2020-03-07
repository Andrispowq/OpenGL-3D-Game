#include "GLContext.h"
#include "glew.h"
#include "glfw3.h"
#include <stdio.h>

bool GLContext::InitContext()
{
	GLenum error = glewInit();

	glewExperimental = GL_FALSE;

	if (error != GLEW_OK)
	{
		printf("The initialisation of OpenGL Extension Wrangler Library (GLEW) has failed! The error \n%s\n", glewGetErrorString(error));

		glfwTerminate();
		return false;
	}

	return true;
}

bool GLContext::DeleteContext()
{
	return true;
}