#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLAtmosphereScatteringShader.h"

#include "engine/prehistoric/core/gameObject/GameObject.h"

#include "engine/prehistoric/modules/atmosphere/Atmosphere.h"

GLAtmosphereScatteringShader::GLAtmosphereScatteringShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/atmosphere/scattering_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/atmosphere/scattering_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("m_transform");
	AddUniform("m_view");
	AddUniform("m_projection");

	AddUniform("sunPosition");
	AddUniform("sunRadius");
	AddUniform("sunColour");
	AddUniform("sunIntensity");

	AddUniform("bloom");
	AddUniform("horizontalVerticalShift");
	AddUniform("width");
	AddUniform("height");
	AddUniform("isReflection");
}

void GLAtmosphereScatteringShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights, uint32_t instance_index) const
{
	SetUniform("m_view", camera->getViewMatrix());
	SetUniform("m_projection", camera->getProjectionMatrix());

	SetUniformf("sunRadius", AtmosphereConfig::sunRadius);
	SetUniform("sunColour", AtmosphereConfig::sunColour);
	SetUniformf("sunIntensity", AtmosphereConfig::sunIntensity);

	SetUniformf("bloom", AtmosphereConfig::bloomFactor);
	SetUniformf("horizontalVerticalShift", AtmosphereConfig::horizontalVerticalShift);
	SetUniformi("width", FrameworkConfig::windowWidth);
	SetUniformi("height", FrameworkConfig::windowHeight);
	SetUniformi("isReflection", 0); //TODO: getting if it's a reflection or not from the RenderingEngine
}

void GLAtmosphereScatteringShader::UpdateObjectUniforms(GameObject* object, uint32_t instance_index) const
{
	SetUniform("m_transform", object->getWorldTransform()->getTransformationMatrix());
	SetUniform("sunPosition", ((Atmosphere*)object)->getSunPosition());
}
