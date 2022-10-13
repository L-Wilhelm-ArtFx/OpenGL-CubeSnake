#pragma once
#include <GLFW/glfw3.h>

class GWindow {
	private:
	GLFWwindow *window = nullptr;

	public:
	GWindow();
	~GWindow();

	void create();
	void destroy();

	bool isValid();

	void swapBuffers();

	bool shoudClose();

	void getFrameSize(int *width, int *height);
};