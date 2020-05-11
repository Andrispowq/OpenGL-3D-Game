#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLTerrainShader.h"

#include "engine/prehistoric/modules/terrain/TerrainNode.h"

GLTerrainShader::GLTerrainShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_TC.glsl"), TESSELLATION_CONTROL_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_TE.glsl"), TESSELLATION_EVALUATION_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_GS.glsl"), GEOMETRY_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_FS.glsl"), FRAGMENT_SHADER);
	CompileShader();

	AddUniform("heightmap");

	AddUniform("localMatrix");
	AddUniform("worldMatrix");

	AddUniform("cameraPosition");
	AddUniform("location");
	AddUniform("index");
	AddUniform("scaleY");
	AddUniform("gap");
	AddUniform("lod");

	for (unsigned int i = 0; i < 8; i++)
	{
		std::string uName = "lodMorphArea[" + std::to_string(i) + "]";

		AddUniform(uName);
	}

	AddUniform("tessellationFactor");
	AddUniform("tessellationSlope");
	AddUniform("tessellationShift");

	AddUniform("viewProjection");

	AddUniform("normalmap");
	AddUniform("splatmap");

	for (unsigned int i = 0; i < 3; i++)
	{
		std::string uniformName = "materials[" + std::to_string(i) + "].";

		AddUniform(uniformName + ALBEDO_MAP);
		AddUniform(uniformName + NORMAL_MAP);
		AddUniform(uniformName + DISPLACEMENT_MAP);
		AddUniform(uniformName + METALLIC_MAP);
		AddUniform(uniformName + ROUGHNESS_MAP);
		//AddUniform(uniformName + OCCLUSION_MAP);

		AddUniform(uniformName + HEIGHT_SCALE);
		AddUniform(uniformName + HORIZONTAL_SCALE);
		AddUniform(uniformName + METALLIC);
		AddUniform(uniformName + ROUGHNESS);
		//AddUniform(uniformName. + OCCLUSION);
	}

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string uniformName = "lights[" + std::to_string(i) + "].";

		AddUniform(uniformName + "position");
		AddUniform(uniformName + "colour");
		AddUniform(uniformName + "intensity");
	}

	AddUniform("highDetailRange");
	AddUniform("gamma");
	AddUniform("numberOfLights");

	//AddUniform("irradianceMap");
	//AddUniform("prefilterMap");
	//AddUniform("brdfLUT");
}

void GLTerrainShader::UpdateShaderUniforms(Camera* camera, const std::vector<Light*>& lights) const
{
	SetUniform("viewProjection", camera->getViewProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string uniformName = "lights[" + std::to_string(i) + "].";

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform(uniformName + "position", light->GetParent()->GetWorldTransform()->GetPosition());
			SetUniform(uniformName + "colour", light->GetColour());
			SetUniform(uniformName + "intensity", light->GetIntensity());
		}
		else
		{
			SetUniform(uniformName + "position", Vector3f());
			SetUniform(uniformName + "colour", Vector3f());
			SetUniform(uniformName + "intensity", Vector3f());
		}
	}

	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformi("numberOfLights", (uint32_t)lights.size() < EngineConfig::lightsMaxNumber ? (uint32_t)lights.size() : EngineConfig::lightsMaxNumber);

	//Some other stuff that is terrain-related
	SetUniformf("scaleY", TerrainConfig::scaleY);

	SetUniformi("tessellationFactor", TerrainConfig::tessellationFactor);
	SetUniformf("tessellationSlope", TerrainConfig::tessellationSlope);
	SetUniformf("tessellationShift", TerrainConfig::tessellationShift);

	for (unsigned int i = 0; i < 8; i++)
	{
		std::string uName = "lodMorphArea[" + std::to_string(i) + "]";

		SetUniformi(uName, TerrainConfig::lodMorphingAreas[i]);
	}

	uint32_t texUnit = 3;

	for (unsigned int i = 0; i < 3; i++)
	{
		std::string uniformName = "materials[" + std::to_string(i) + "].";

		Material* material = TerrainConfig::materials[i];

		material->GetTexture(ALBEDO_MAP)->Bind(texUnit);
		SetUniformi(uniformName + ALBEDO_MAP, texUnit);
		texUnit++;

		material->GetTexture(NORMAL_MAP)->Bind(texUnit);
		SetUniformi(uniformName + NORMAL_MAP, texUnit);
		texUnit++;

		material->GetTexture(DISPLACEMENT_MAP)->Bind(texUnit);
		SetUniformi(uniformName + DISPLACEMENT_MAP, texUnit);
		texUnit++;

		material->GetTexture(METALLIC_MAP)->Bind(texUnit);
		SetUniformi(uniformName + METALLIC_MAP, texUnit);
		texUnit++;

		material->GetTexture(ROUGHNESS_MAP)->Bind(texUnit);
		SetUniformi(uniformName + ROUGHNESS_MAP, texUnit);
		texUnit++;

		SetUniformf(uniformName + HEIGHT_SCALE, material->GetFloat(HEIGHT_SCALE));
		SetUniformf(uniformName + HORIZONTAL_SCALE, material->GetFloat(HORIZONTAL_SCALE));
		SetUniformf(uniformName + METALLIC, material->GetFloat(METALLIC));
		SetUniformf(uniformName + ROUGHNESS, material->GetFloat(ROUGHNESS));
		//SetUniformf(uniformName + OCCLUSION, material->GetFloat(OCCLUSION));
	}
}

void GLTerrainShader::UpdateSharedUniforms(GameObject* object) const
{
	TerrainNode* node = (TerrainNode*)object;

	node->getMaps()->getHeightmap()->Bind(0);
	SetUniformi("heightmap", 0);
	node->getMaps()->getNormalmap()->Bind(1);
	SetUniformi("normalmap", 1);
	node->getMaps()->getSplatmap()->Bind(2);
	SetUniformi("splatmap", 2);
}

void GLTerrainShader::UpdateObjectUniforms(GameObject* object) const
{
	TerrainNode* node = (TerrainNode*)object;

	SetUniform("localMatrix", object->GetLocalTransform()->getTransformationMatrix());
	SetUniform("worldMatrix", object->GetWorldTransform()->getTransformationMatrix());

	SetUniform("location", node->getLocation());
	SetUniform("index", node->getIndex());
	SetUniformf("gap", node->getGap());
	SetUniformi("lod", node->getLod());
}