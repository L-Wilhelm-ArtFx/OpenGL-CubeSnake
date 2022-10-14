#include "window.h"
#include "input.h"

GWindow::GWindow()
{
}

GWindow::~GWindow()
{
	destroy();
}

void GWindow::create()
{
	destroy();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, 1);
	window = glfwCreateWindow(1280, 720, "OpenGL Cubes", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, GInput::glfwKeyCallback);
}

void GWindow::destroy()
{
	if (isValid())
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}
}

bool GWindow::isValid()
{
	return window != nullptr;
}

void GWindow::swapBuffers()
{
	glfwSwapBuffers(this->window);
}

bool GWindow::shoudClose()
{
	return isValid() && glfwWindowShouldClose(window);
}

void GWindow::getFrameSize(int *width, int *height) {
	glfwGetFramebufferSize(window, width, height);
}