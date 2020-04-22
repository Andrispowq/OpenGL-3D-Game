#include "engine/prehistoric/core/util/Includes.hpp"
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

float* Mesh::GetVertexData() const
{
	const size_t numFloats = Vertex::GetNumberOfFloats();

	float* data = new float[vertices.size() * numFloats];
	int counter = 0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		counter = 0;

		data[i * numFloats + counter++] = vertices[i].getPosition().x;
		data[i * numFloats + counter++] = vertices[i].getPosition().y;
		data[i * numFloats + counter++] = vertices[i].getPosition().z;
		data[i * numFloats + counter++] = vertices[i].getTexture().x;
		data[i * numFloats + counter++] = vertices[i].getTexture().y;
		data[i * numFloats + counter++] = vertices[i].getNormal().x;
		data[i * numFloats + counter++] = vertices[i].getNormal().y;
		data[i * numFloats + counter++] = vertices[i].getNormal().z;
	}
	
	return data;
}

uint16_t* Mesh::GetIndexData() const
{
	uint16_t* indexArray = new uint16_t[indices.size()];

	for (unsigned int i = 0; i < indices.size(); i++)
	{
		indexArray[i] = indices[i];
	}

	return indexArray;
}