#ifndef SHADER_H
#define SHADER_H

#include "engine/prehistoric/core/util/Includes.hpp"

#include "engine/prehistoric/core/math/Math.h"

enum ShaderType
{
	VERTEX_SHADER = 0x00000001,
	TESSELLATION_CONTROL_SHADER = 0x00000002,
	TESSELLATION_EVALUATION_SHADER = 0x00000004,
	GEOMETRY_SHADER = 0x00000008,
	FRAGMENT_SHADER = 0x00000010,
	COMPUTE_SHADER = 0x00000020,
	GRAPHICS_PIPELINE = 0x0000001F,
	RAY_GENERATION_SHADER_NV = 0x00000040,
	RAY_HIT_SHADER_NV = 0x00000080,
	RAY_CLOSEST_HIT_SHADER_NV = 0x00000100,
	RAY_INTERSECTION_SHADER_NV = 0x00000200,
	RAY_CALLABLE_SHADER_NV = 0x00000400,
	RAY_MISS_SHADER_NV = 0x00000800,
	TASK_SHADER_NV = 0x00001000,
	MESH_SHADER_NV = 0x00002000,
	MESH_SHADING_PIPELINE_NV = 0x00003000,
	UNKNOWN = 0xFFFFFFFF
};

enum ShaderCodeType
{
	GLSL,
	HLSL,
	SPIR_V_BINARY,
	SPIR_V_ASSEMBLY
};

namespace ResourceLoader
{
	std::vector<char> LoadShaderGL(const std::string& filename);
	std::vector<char> LoadShaderVK(const std::string& filename);
};

class GameObject;
class Camera;
class Light;

class Shader
{
public:
	Shader() {}
	virtual ~Shader() = 0;

	virtual void Bind(void* commandBuffer) const = 0;
	virtual void Unbind() const = 0;

	virtual bool AddUniform(const std::string& name) = 0;
	virtual bool AddUniformBlock(const std::string& name) = 0;

	virtual bool AddShader(const std::vector<char>& code, ShaderType type) = 0;
	virtual bool CompileShader() const = 0;

	//Uniform handling
	virtual void SetUniformi(const std::string& name, int value) const = 0;
	virtual void SetUniformf(const std::string& name, float value) const = 0;

	virtual void SetUniform(const std::string& name, const Vector2f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Vector3f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Vector4f& value) const = 0;
	virtual void SetUniform(const std::string& name, const Matrix4f& value) const = 0;

	virtual void BindUniformBlock(const std::string& name, uint32_t binding) const = 0;

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const = 0;
	
	//Shaders cannot be copied, they are stored as pointers in the Renderer component, and referenced with a shaderIndex
	//Shader(const Shader&) = delete;
	//Shader operator=(const Shader&) = delete;
protected:
	std::unordered_map<std::string, uint32_t> uniforms;
	mutable uint32_t counter;
};

#endif