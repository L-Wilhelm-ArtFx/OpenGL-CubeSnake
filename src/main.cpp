#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"

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
		std::cout << "Failed" << std::endl;
		return 1;
	}
	std::cout << "Done" << std::endl;

	GWindow win;
	win.create();

	std::cout << "INIT Glew:" << std::endl;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cout << "Failed" << std::endl;
		fprintf(stderr, "Glew Error: %s\n", glewGetErrorString(glewStatus));
		return 1;
	}
	std::cout << "Done" << std::endl;

	std::chrono::high_resolution_clock clock;

	auto timePtNow = clock.now();
	auto timePt = timePtNow;
	delta_dur_t delta(0);
	delta_dur_t stepDelta(0);
	delta_dur_t totalTime(0);
	const delta_dur_t stepDuration(1000000000LL / 60LL);
	const delta_dur_t maxDelta(1000000000LL);
	const delta_dur_t minDelta(1000000000LL / 240LL);

	float vertices[] = {
		-2.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	const char *vertexShaderSource = "#version 440 core\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "void main()\n"
									 "{\n"
									 "   vec3 a[3] = vec3[](vec3(-1.0f,0.0f,0.5f), vec3(0.5f,0.5f,0.0f), vec3(0.0f,1.0f,-0.5f));"
									 "   vec3 pos = a[gl_VertexID] + aPos;"
									 "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
									 "}\0";

	GLuint vShad;
	vShad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShad, 1, &vertexShaderSource, NULL);
	glCompileShader(vShad);
	GLint vSuccess;
	glGetShaderiv(vShad, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		char infoLog[1024];
		glGetShaderInfoLog(vShad, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	const char *fragmentShaderSource = "#version 440 core\n"
									   "out vec4 FragColor;\n"
									   "void main()\n"
									   "{\n"
									   "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									   "}\0";
	GLuint fShad;
	fShad = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShad, 1, &fragmentShaderSource, NULL);
	glCompileShader(fShad);
	GLint fSuccess;
	glGetShaderiv(fShad, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(fShad, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	GLuint prog;
	prog = glCreateProgram();
	glAttachShader(prog, vShad);
	glAttachShader(prog, fShad);
	glLinkProgram(prog);
	GLint pSuccess;
	glGetProgramiv(prog, GL_COMPILE_STATUS, &pSuccess);
	if (!pSuccess)
	{
		char infoLog[512];
		glGetProgramInfoLog(prog, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}

	glUseProgram(prog);

	glDeleteShader(vShad);
	glDeleteShader(fShad);

	while (!win.shoudClose())
	{
		glfwPollEvents();
		timePtNow = clock.now();

		stepDelta = std::chrono::duration_cast<delta_dur_t>(timePtNow - timePt);
		if (stepDelta >= minDelta)
		{
			int width, height;
			win.getFrameSize(&width, &height);

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
				// === UPDATE ===
				glClearColor(((float)sin(std::chrono::duration<double>(totalTime).count()) + 1.0f) * 0.5f, 0.6f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUseProgram(prog);

				glBindVertexArray(vao);

				glDrawArrays(GL_TRIANGLES, 0, 3);

				checkGLErr();

				totalTime += stepDuration;

				win.swapBuffers();
			}
		}
		else
		{
		}
	}

	win.destroy();

	glfwTerminate();

	return 0;
}
