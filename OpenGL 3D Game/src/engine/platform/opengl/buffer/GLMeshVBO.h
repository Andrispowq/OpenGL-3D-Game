#ifndef VBO_H
#define VBO_H

#include <glew.h>

#include "engine/prehistoric/common/buffer/MeshVBO.h"

class GLMeshVBO : public MeshVBO
{
public:
	GLMeshVBO(const Mesh& mesh);
	GLMeshVBO() : MeshVBO(), vao(0), vbo(0), ibo(0) {}

	virtual ~GLMeshVBO() override;

	void Store(const Mesh& mesh) override;

	void Bind() const override;
	void Draw() const override;
	void Unbind() const override;
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

#endif