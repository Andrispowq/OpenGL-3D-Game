#include "GLWindow.h"
#include "context/GLContext.h"

GLWindow::~GLWindow()
{
	glfwDestroyWindow(window);
	Context::GetContext().DeleteContext();
	glfwTerminate();
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool GLWindow::Create()
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FrameworkConfig::apiVersion.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FrameworkConfig::apiVersion.y);
	glfwWindowHint(GLFW_RESIZABLE, FrameworkConfig::windowResizable);
	glfwWindowHint(GLFW_SAMPLES, FrameworkConfig::windowNumSamples);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	closed = false;

	//Checking if a valid window was created
	if (window == NULL)
	{
		std::cout << "The creation of the window has failed!" << std::endl;

		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	//Initialisation of GLEW
	if (!Context::GetContext().InitContext())
	{
		glfwTerminate();
		return false;
	}

	glfwShowWindow(window);
	glfwSwapInterval(1);

	return true;
}

bool GLWindow::initGLFW() const
{
	if (!glfwInit())
	{
		std::cout << "The initialisiation of OpenGL Framework (GLFW) has failed!" << std::endl;
		return false;
	}

	return true;
}

void GLWindow::ClearColor(const float& red, const float& green, const float& blue, const float& alpha) const
{
	glClearColor(red, green, blue, alpha);
}

void GLWindow::Input()
{
	glfwPollEvents();
}

void GLWindow::ClearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::Render() const
{
	glfwSwapBuffers(window);
}

bool GLWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window) || closed;
}