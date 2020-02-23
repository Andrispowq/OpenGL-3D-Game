#ifndef VBO_H
#define VBO_H

#include <glew.h>
#include "engine/core/model/Mesh.h"

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

#endif