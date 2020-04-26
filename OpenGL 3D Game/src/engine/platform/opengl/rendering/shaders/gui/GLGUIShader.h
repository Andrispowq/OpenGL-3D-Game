#ifndef GL_GUI_SHADER_H
#define GL_GUI_SHADER_H

#include <glew.h>

#include "engine/platform/opengl/rendering/shaders/GLShader.h"

class GLGUIShader : public GLShader
{
public:
	GLGUIShader();
	virtual ~GLGUIShader() {}

	virtual void UpdateShaderUniforms(Camera* camera, std::vector<Light*> lights) const override {}
	virtual void UpdateObjectUniforms(GameObject* object) const override;
};

#endif