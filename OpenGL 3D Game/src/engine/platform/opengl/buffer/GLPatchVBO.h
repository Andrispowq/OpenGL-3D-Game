#ifndef GL_PATCH_VBO_H
#define GL_PATCH_VBO_H

#include <glew.h>

#include "engine/prehistoric/common/buffer/PatchVBO.h"

class GLPatchVBO : public PatchVBO
{
public:
	GLPatchVBO(const std::vector<Vector2f>& vertices);
	GLPatchVBO() : PatchVBO(), vao(0), vbo(0) {}

	virtual ~GLPatchVBO() override;

	void Store(const std::vector<Vector2f>& vertices) override;

	void Bind(void* commandBuffer) const override;
	void Draw(void* commandBuffer) const override;
	void Unbind() const override;
private:
	GLuint vao;
	GLuint vbo;
};

#endif