#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glew.h>
#include <fstream>

#include "engine/prehistoric/common/rendering/shaders/Shader.h"
#include "engine/prehistoric/component/light/Light.h"

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

	virtual void UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const override = 0;

	virtual void SetUniformi(const std::string& name, int value) const override { glUniform1i(uniforms.at(name), value); }
	virtual void SetUniformf(const std::string& name, float value) const override { glUniform1f(uniforms.at(name), value); }

	virtual void SetUniform(const std::string& name, const Vector2f& value) const override { glUniform2f(uniforms.at(name), value.x, value.y); }
	virtual void SetUniform(const std::string& name, const Vector3f& value) const override { glUniform3f(uniforms.at(name), value.x, value.y, value.z); }
	virtual void SetUniform(const std::string& name, const Vector4f& value) const override { glUniform4f(uniforms.at(name), value.x, value.y, value.z, value.w); }
	virtual void SetUniform(const std::string& name, const Matrix4f& matrix) const override { glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, matrix.m); }

	virtual void BindUniformBlock(const std::string& name, unsigned int binding) const override { glUniformBlockBinding(program, uniforms.at(name), binding); }

	void BindFragDataLocation(const std::string& name, GLuint index)
	{
		glBindFragDataLocation(program, index, name.c_str());
	}
private:
	bool AddProgram(const std::string& code, GLenum type) const;
private:
	GLuint program;
	GLuint* shaders;
};

#endif 