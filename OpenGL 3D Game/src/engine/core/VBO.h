#pragma once

#include <glew.h>
#include "Mesh.h"

class VBO
{
public:
	VBO(const Mesh& mesh);
	VBO() {}

	virtual ~VBO();

	void store(const Mesh& mesh);
	void draw() const;
private:
	unsigned int size;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};