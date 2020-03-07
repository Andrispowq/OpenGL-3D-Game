#include "Window.h"

Window::~Window()
{
	Context::GetContext().DeleteContext();
}