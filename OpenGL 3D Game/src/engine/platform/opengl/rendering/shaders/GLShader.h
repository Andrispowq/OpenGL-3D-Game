#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include <glew.h>

#include <fstream>

#include "engine/prehistoric/component/light/Light.h"

#include "engine/prehistoric/core/util/Util.h"

class GLShader : public Shader
{
public:
	GLShader(const std::vector<char>* files, uint32_t length);
	GLShader();

	virtual ~GLShader() override;

	void Bind(void* commandBuffer) const override;
	void Unbind() const override;

	bool AddUniform(const std::string& name, uint32_t stages = GRAPHICS_PIPELINE, UniformType type = UniformBuffer, uint32_t set = 0, uint32_t binding = 0, size_t size = 0, Texture* texture = nullptr) override;
	bool AddUniformBlock(const std::string& name, uint32_t stages = GRAPHICS_PIPELINE, UniformType type = UniformBuffer, uint32_t set = 0, uint32_t binding = 0, size_t size = 0, Texture* texture = nullptr) override;
	void BindAttribute(const std::string& name, GLuint location) const;

	bool AddShader(const std::vector<char>& code, ShaderType type) override;
	bool CompileShader() const override;

	virtual void UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights) const override {};
	virtual void UpdateSharedUniforms(GameObject* object) const override {}
	virtual void UpdateObjectUniforms(GameObject* object) const override {};

	virtual void SetUniformi(const std::string& name, int value, size_t offset = 0) const override { glUniform1i(getUniformLocation(name), value); }
	virtual void SetUniformf(const std::string& name, float value, size_t offset = 0) const override { glUniform1f(getUniformLocation(name), value); }

	virtual void SetUniform(const std::string& name, const Vector2f& value, size_t offset = 0) const override { glUniform2f(getUniformLocation(name), value.x, value.y); }
	virtual void SetUniform(const std::string& name, const Vector3f& value, size_t offset = 0) const override { glUniform3f(getUniformLocation(name), value.x, value.y, value.z); }
	virtual void SetUniform(const std::string& name, const Vector4f& value, size_t offset = 0) const override { glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	virtual void SetUniform(const std::string& name, const Matrix4f& matrix, size_t offset = 0) const override { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.m); }

	virtual void SetTexture(const std::string& name, Texture* value) const {};

	virtual void SetUniform(const std::string& name, const void* value, size_t size, size_t offset = 0) const override {}

	virtual void BindUniformBlock(const std::string& name, uint32_t binding) const override { glUniformBlockBinding(program, getUniformLocation(name), binding); }

	void BindFragDataLocation(const std::string& name, GLuint index)
	{
		glBindFragDataLocation(program, index, name.c_str());
	}

	virtual bool operator==(const Shader& other)
	{
		return program == (*reinterpret_cast<const GLShader*>(&other)).program;
	}
private:
	bool AddProgram(const std::vector<char>& code, GLenum type) const;

	uint32_t getUniformLocation(const std::string& name) const
	{
		return uniforms.at(name);
	}
private:
	std::map<std::string, uint32_t> uniforms;

	GLuint program;
	GLuint* shaders;
};

#endif 