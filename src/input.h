#pragma once

#include <unordered_map>
#include <GLFW/glfw3.h>

namespace GInput {

	void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool isPressed(int key);
}