#pragma once

#include <vector>
#include <GL/glew.h>

class Mesh {
	private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	unsigned int vertCount = 0;

	public:
	Mesh();
	~Mesh();
	Mesh(const Mesh&) = delete;

	void load(const std::vector<float> &verts, const std::vector<unsigned int> &indices);

	void use();
	void draw();
	void destroy();
	GLuint getVaoID();
	unsigned int getVertCount();


};