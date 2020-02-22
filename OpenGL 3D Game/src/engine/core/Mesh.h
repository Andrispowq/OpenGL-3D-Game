#pragma once

#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(const Mesh& mesh);

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices) : vertices(vertices), indices(indices) {}
	Mesh() : vertices(NULL), indices(NULL) {}

	virtual ~Mesh();

	void addVertex(const Vertex& vertex) { vertices.push_back(vertex); }
	void addIndex(const int& index) { indices.push_back(index); }

	inline std::vector<Vertex> getVertices() const { return vertices; }
	inline std::vector<unsigned short> getIndices() const { return indices; }

	inline void setVertices(std::vector<Vertex> vertices) { this->vertices = vertices; }
	inline void setIndices(std::vector<unsigned short> indices) { this->indices = indices; }
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};