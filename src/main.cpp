#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "shader.h"
#include "game.h"
#include "mesh.h"
#include "glm_includes.h"
#include "input.h"

typedef std::chrono::nanoseconds delta_dur_t;

void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error: %s\n", description);
}

void checkGLErr()
{
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		std::string errName;
		switch (errCode)
		{
		case GL_INVALID_ENUM:
			errName = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errName = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errName = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errName = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errName = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errName = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errName = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errName = "UNKNOWN_ERR";
			break;
		}
		fprintf(stderr, "GL Error(%u): %s", errCode, errName.c_str());
	}
}

int main(int argc, char const *argv[])
{
	std::cout << "INIT" << std::endl;

	std::cout << "INIT Glfw:" << std::endl;
	glfwSetErrorCallback(glfw_error_callback);
	int glfwErr = glfwInit();
	if (!glfwErr)
	{
		std::cerr << "Failed" << std::endl;
		return 1;
	}
	std::cout << "Done" << std::endl;

	GWindow win;
	win.create();

	std::cout << "INIT Glew:" << std::endl;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cerr << "Failed" << std::endl;
		fprintf(stderr, "Glew Error: %s\n", glewGetErrorString(glewStatus));
		return 1;
	}
	std::cout << "Done" << std::endl;

	// Loop Timing

	std::chrono::high_resolution_clock clock;

	auto timePtNow = clock.now();
	auto timePt = timePtNow;
	delta_dur_t delta(0);
	delta_dur_t stepDelta(0);
	delta_dur_t totalTime(0);
	const delta_dur_t stepDuration(1000000000LL / 60LL);
	const delta_dur_t maxDelta(1000000000LL);
	const delta_dur_t minDelta(1000000000LL / 240LL);

	// Game Loop
	printf("Starting Game Loop\n");

	MeshInstance inst;
	GameObj camera;

	camera.pos[2] = -3.0f;

	GameScene scene;

	scene.start();

	glEnable(GL_DEPTH_TEST);

	while (!win.shoudClose())
	{
		glfwPollEvents();
		timePtNow = clock.now();

		stepDelta = std::chrono::duration_cast<delta_dur_t>(timePtNow - timePt);
		if (stepDelta >= minDelta)
		{
			int width, height;
			win.getFrameSize(&width, &height);
			scene.width = width;
			scene.height = height;
			glViewport(0, 0, width, height);
			timePt = timePtNow;
			delta += stepDelta;
			if (delta >= maxDelta)
			{
				delta = maxDelta;
			}

			for (unsigned int i = 0; i < 10 && delta.count() > 0; i++)
			{
				delta -= stepDuration;

				scene.update();

				checkGLErr();

				totalTime += stepDuration;

				win.swapBuffers();
			}
		}
		else
		{
			
		}
	}

	// Clean up
	scene.stop();
	win.destroy();

	glfwTerminate();

	return 0;
}
