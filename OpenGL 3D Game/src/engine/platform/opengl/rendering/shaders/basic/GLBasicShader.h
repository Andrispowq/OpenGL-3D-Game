#ifndef GL_BASIC_SHADER_H
#define GL_BASIC_SHADER_H

#include "engine/platform/opengl/rendering/shaders/GLShader.h"

#include <glew.h>

#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/movement/Camera.h"
#include "engine/prehistoric/component/renderer/Renderer.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/common/model/Texture.h"

#include "engine/config/EngineConfig.h"

class GLBasicShader : public GLShader
{
public:
	GLBasicShader();
	virtual ~GLBasicShader() {}

	virtual void UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights) const override;
	virtual void UpdateObjectUniforms(GameObject* object) const override;
};

#endif