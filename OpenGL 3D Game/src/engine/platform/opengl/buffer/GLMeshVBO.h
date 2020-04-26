#ifndef GL_MESH_VBO_H
#define GL_MESH_VBO_H

#include <glew.h>

#include "engine/prehistoric/common/buffer/MeshVBO.h"

class GLMeshVBO : public MeshVBO
{
public:
	GLMeshVBO(const Mesh& mesh);
	GLMeshVBO() : MeshVBO(), vao(0), vbo(0), ibo(0) {}

	virtual ~GLMeshVBO() override;

	void Store(const Mesh& mesh) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;

	virtual bool operator==(const VBO& other) override
	{
		return vao == (*reinterpret_cast<const GLMeshVBO*>(&other)).vao && vbo == (*reinterpret_cast<const GLMeshVBO*>(&other)).vbo
			&& ibo == (*reinterpret_cast<const GLMeshVBO*>(&other)).ibo;
	}
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

#endif