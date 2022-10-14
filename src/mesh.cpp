#include "mesh.h"

#include <iostream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	destroy();
}

void Mesh::load(const std::vector<float> &verts, const std::vector<unsigned int> &indices)
{
	destroy();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertCount = indices.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void Mesh::use()
{
	glBindVertexArray(vao);
}

void Mesh::draw()
{
	use();
	glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);
}

void Mesh::destroy()
{
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
	}
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	if (ebo)
	{
		glDeleteBuffers(1, &ebo);
	}
	vertCount = 0;
}

GLuint Mesh::getVaoID()
{
	return vao;
}