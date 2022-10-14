#include "input.h"
#include <iostream>

std::unordered_map<int, bool> keyMap;

void GInput::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;
	keyMap.insert_or_assign(key, pressed);
	printf("KEY %d %d\n", key, pressed);
}

bool GInput::isPressed(int key)
{
	auto k = keyMap.find(key);
	return k != keyMap.end() && (*k).second;
}