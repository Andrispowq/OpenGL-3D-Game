#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLMeshVBO.h"

GLMeshVBO::GLMeshVBO(const Mesh& mesh) : MeshVBO()
{
	Store(mesh);
}

GLMeshVBO::~GLMeshVBO()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void GLMeshVBO::Store(const Mesh& mesh)
{
	size = static_cast<unsigned int>(mesh.getIndices().size());

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	const int data_length = 8;
	float* vdata = new float[mesh.getVertices().size() * data_length];

	for (size_t i = 0; i < mesh.getVertices().size(); i++)
	{
		Vertex vertex = mesh.getVertices()[i];

		vdata[data_length * i + 0] = vertex.getPosition().x;
		vdata[data_length * i + 1] = vertex.getPosition().y;
		vdata[data_length * i + 2] = vertex.getPosition().z;
		vdata[data_length * i + 3] = vertex.getTexture().x;
		vdata[data_length * i + 4] = vertex.getTexture().y;
		vdata[data_length * i + 5] = vertex.getNormal().x;
		vdata[data_length * i + 6] = vertex.getNormal().y;
		vdata[data_length * i + 7] = vertex.getNormal().z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.getVertices()[0]) * mesh.getVertices().size(), (void*) &vdata[0], GL_STATIC_DRAW);

	unsigned short* idata = new unsigned short[mesh.getIndices().size()];

	for (size_t i = 0; i < mesh.getIndices().size(); i++)
	{
		idata[i] = mesh.getIndices()[i];
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.getIndices()[0]) * mesh.getIndices().size(), (void*) &idata[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 5));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLMeshVBO::Bind() const
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void GLMeshVBO::Draw() const
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, (void*) 0);
}

void GLMeshVBO::Unbind() const
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}