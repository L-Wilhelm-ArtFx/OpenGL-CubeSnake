#include "shader.h"
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

GLuint ShaderProgram::getProgramId()
{
	return progId;
}

bool ShaderProgram::load(const GLchar *vertexSrc, const GLchar *fragmentSrc)
{
	GLuint shaderV = createShader(vertexSrc, GL_VERTEX_SHADER);
	GLuint shaderF = createShader(fragmentSrc, GL_FRAGMENT_SHADER);
	if (shaderV == 0 || shaderF == 0)
	{
		fprintf(stderr, "Aborting ShaderProgram linking.");
		return false;
	}

	progId = glCreateProgram();
	glAttachShader(progId, shaderV);
	glAttachShader(progId, shaderF);

	glLinkProgram(progId);

	GLint linkStatus;
	glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		char infoLog[1024];
		glGetProgramInfoLog(progId, 1024, NULL, infoLog);
		fprintf(stderr, "Failed to link ShaderProgram:\n%s", infoLog);
		glDeleteProgram(progId);
		progId = 0;
	}

	glDeleteShader(shaderV);
	glDeleteShader(shaderF);

	return progId != 0;
}

GLuint ShaderProgram::createShader(const GLchar *src, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string shaderTypeName = "Unknown Shader";
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			shaderTypeName = "Vertex Shader";
			break;
		case GL_FRAGMENT_SHADER:
			shaderTypeName = "Fragment Shader";
			break;
		}
		fprintf(stderr, "%s compilation failed:\n%s", shaderTypeName.c_str(), infoLog);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void ShaderProgram::use() {
	glUseProgram(progId);
}

void ShaderProgram::destroy() {
	if(progId==0)return;
	glDeleteProgram(progId);
}