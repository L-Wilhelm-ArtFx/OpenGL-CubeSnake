#pragma once
#include <GL/glew.h>
#include <memory>

class ShaderProgram
{
public:
private:
	GLuint progId = 0;

public:
	ShaderProgram();

	bool load(const GLchar *vertexSrc, const GLchar *fragmentSrc);
	void use();
	void destroy();
	GLuint getProgramId();

private:
	GLuint createShader(const GLchar *src, GLenum shaderType);
};