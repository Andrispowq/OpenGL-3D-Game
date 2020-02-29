#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H

#include "engine/engines/rendering/shaders/Shader.h"

class BasicShader : public Shader
{
public:
	BasicShader();
	virtual ~BasicShader() {}

	virtual void UpdateUniforms(GameObject* object, Camera* camera) const override;

	BasicShader(const BasicShader&) = delete;
	BasicShader operator=(const BasicShader&) = delete;
};

#endif