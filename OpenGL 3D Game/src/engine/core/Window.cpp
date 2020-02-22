#include "Window.h"

Window::Window()
{
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	title = DEFAULT_TITLE;
	fullscreen = DEFAULT_FULLSCREEN;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool Window::create()
{
	//Initialisation of GLFW
	if (!initGLFW())
	{
		return false;
	}

	//Setting error callback
	glfwSetErrorCallback(error_callback);

	//Setting window properties and creating the window
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	//Checking if a valid window was created
	if (window == NULL)
	{
		std::cout << "The creation of the window has failed!" << std::endl;

		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	//Initialisation of GLEW
	if (!initGLEW())
	{
		glfwTerminate();
		return false;
	}

	glfwShowWindow(window);
	glfwSwapInterval(1);

	return true;
}

bool Window::initGLFW() const
{
	if (!glfwInit())
	{
		std::cout << "The initialisiation of OpenGL Framework (GLFW) has failed!" << std::endl;
		return false;
	}

	return true;
}

bool Window::initGLEW() const
{
	GLenum error = glewInit();

	glewExperimental = GL_FALSE;

	if (error != GLEW_OK)
	{
		std::cout << "The initialisation of OpenGL Extension Wrangler Library (GLEW) has failed! The error \n" << glewGetErrorString(error) << std::endl;

		glfwTerminate();
		return false;
	}

	return true;
}

void Window::clearColor(const float& red, const float& green, const float& blue, const float& alpha) const
{
	glClearColor(red, green, blue, alpha);
}

void Window::input()
{
	glfwPollEvents();
}

void Window::clearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::render() const
{
	glfwSwapBuffers(window);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}