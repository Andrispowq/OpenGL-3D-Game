#include "Mesh.h"

Mesh::Mesh(const Mesh& mesh)
{
	this->vertices = mesh.vertices;
	this->indices = mesh.indices;
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
}