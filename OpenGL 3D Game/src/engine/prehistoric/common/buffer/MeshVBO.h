#ifndef MESH_VBO_H
#define MESH_VBO_H

#include "engine/prehistoric/core/model/Mesh.h"

class MeshVBO
{
public:
	MeshVBO() : size(0) {}
	
	virtual ~MeshVBO() = 0;

	virtual void Store(const Mesh& mesh) = 0;

	virtual void Bind() const = 0;
	virtual void Draw() const = 0;
	virtual void Unbind() const = 0;
protected:
	unsigned int size;
};

#endif