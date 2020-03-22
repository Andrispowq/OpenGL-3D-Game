#ifndef SHADER_H
#define SHADER_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/core/math/Math.h"

enum ShaderType
{
	VERTEX_SHADER,
	TESSELLATION_CONTROL_SHADER,
	TESSELLATION_EVALUATION_SHADER,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER,
	COMPUTE_SHADER
};

namespace ResourceLoader
{
	std::string LoadShader(const std::string& filename);
};

class GameObject;
class Camera;
class Light;

class Shader
{
public:
	Shader() {}
	virtual ~Shader() = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual bool AddUniform(const std::string& name) = 0;
	virtual bool AddUniformBlock(const std::string& name) = 0;

	virtual bool AddShader(const std::string& code, ShaderType type) const = 0;
	virtual bool CompileShader() const = 0;

	//Uniform handling
	virtual void SetUniformi(const std::string& name, int value) const = 0;
	virtual void SetUniformf(const std::string& name, float value) const = 0;

	virtual void SetUniform(const std::string& name, const Vector2f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Vector3f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Vector4f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Matrix4f& value) const = 0;

	virtual void BindUniformBlock(const std::string& name, unsigned int binding) const = 0;

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const = 0;
	
	//Shaders cannot be copied, they are stored as pointers in the Renderer component, and referenced with a shaderIndex
	//Shader(const Shader&) = delete;
	//Shader operator=(const Shader&) = delete;
protected:
	std::unordered_map<std::string, uint32_t> uniforms;
	mutable unsigned int counter;
};

#endif