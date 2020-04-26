#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLGUIShader.h"

#include "engine/prehistoric/component/gui/GUIElement.h"

GLGUIShader::GLGUIShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/gui/gui_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/gui/gui_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("image");
}

void GLGUIShader::UpdateObjectUniforms(GameObject* object) const
{
	SetUniform("m_transform", object->GetWorldTransform()->getTransformationMatrix());

	GUIElement* gui = reinterpret_cast<GUIElement*>(object->GetComponent("button"));

	gui->getTexture()->Bind();
	SetUniformi("image", 0);
}