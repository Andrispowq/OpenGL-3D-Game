#ifndef MESH_VBO_H
#define MESH_VBO_H

#include "VBO.h"

#include "engine/prehistoric/core/model/Mesh.h"

class MeshVBO : public VBO
{
public:
	MeshVBO() {}
	
	virtual ~MeshVBO() override = 0;

	virtual void Store(const Mesh& mesh) override = 0;

	virtual void Bind(void* commandBuffer) const override  = 0;
	virtual void Draw(void* commandBuffer) const = 0;
	virtual void Unbind() const override = 0;
};

#endif