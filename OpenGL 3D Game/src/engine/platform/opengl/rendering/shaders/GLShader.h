#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glew.h>
#include <fstream>
#include <unordered_map>

#include "engine/prehistoric/common/rendering/shaders/Shader.h"

#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/util/Util.h"

class GLShader : public Shader
{
public:
	GLShader(const std::string* files, unsigned int length);
	GLShader();

	virtual ~GLShader() override;

	void Bind() const override;
	void Unbind() const override;

	bool AddUniform(const std::string& name) override;
	bool AddUniformBlock(const std::string& name) override;
	void BindAttribute(const std::string& name, GLuint location) const;

	bool AddShader(const std::string& code, ShaderType type) const override;
	bool CompileShader() const override;

	virtual void UpdateUniforms(GameObject* object, Camera* camera) const override = 0;

	void SetUniformi(const std::string& name, int value) const
	{
		glUniform1i(uniforms.at(name), value);
	}

	void SetUniformf(const std::string& name, float value) const
	{
		glUniform1f(uniforms.at(name), value);
	}

	void SetUniform(const std::string& name, const Vector2f& value) const
	{
		glUniform2f(uniforms.at(name), value.x, value.y);
	}

	void SetUniform(const std::string& name, const Vector3f& value) const
	{
		glUniform3f(uniforms.at(name), value.x, value.y, value.z);
	}

	void SetUniform(const std::string& name, const Vector4f& value) const
	{
		glUniform4f(uniforms.at(name), value.x, value.y, value.z, value.w);
	}

	void SetUniform(const std::string& name, const Matrix4f& matrix) const
	{
		glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, matrix.m);
	}

	void BindUniformBlock(const std::string& name, GLuint binding)
	{
		glUniformBlockBinding(program, uniforms.at(name), binding);
	}

	void BindFragDataLocation(const std::string& name, GLuint index)
	{
		glBindFragDataLocation(program, index, name.c_str());
	}
private:
	bool AddProgram(const std::string& code, GLenum type) const;
private:
	GLuint program;
	GLuint* shaders;

	std::unordered_map<std::string, GLuint> uniforms;
	mutable unsigned int counter;
};

#endif 