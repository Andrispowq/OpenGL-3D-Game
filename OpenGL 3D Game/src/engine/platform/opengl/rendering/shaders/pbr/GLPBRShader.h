#ifndef GL_PBR_SHADER_H
#define GL_PBR_SHADER_H

#include "engine/platform/opengl/rendering/shaders/GLShader.h"

class GLPBRShader : public GLShader
{
public:
	GLPBRShader();
	virtual ~GLPBRShader() {}

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const override;
};

#endif