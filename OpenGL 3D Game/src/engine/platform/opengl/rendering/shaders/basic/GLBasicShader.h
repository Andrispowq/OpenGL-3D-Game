#ifndef GL_BASIC_SHADER_H
#define GL_BASIC_SHADER_H

#include "engine/platform/opengl/rendering/shaders/GLShader.h"

class GLBasicShader : public GLShader
{
public:
	GLBasicShader();
	virtual ~GLBasicShader() {}

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const override;
};

#endif