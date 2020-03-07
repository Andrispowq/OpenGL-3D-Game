#ifndef SHADER_H
#define SHADER_H

#include <string>

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

	virtual void UpdateUniforms(GameObject* object, Camera* camera) const = 0;
	
	Shader(const Shader&) = delete;
	Shader operator=(const Shader&) = delete;
private:
};

#endif