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
		AddUniform("lodMorphArea[" + std::to_string(i) + "]");
	}

	AddUniform("tessellationFactor");
	AddUniform("tessellationSlope");
	AddUniform("tessellationShift");

	AddUniform("viewProjection");

	AddUniform("normalmap");
	AddUniform("splatmap");

	for (unsigned int i = 0; i < 3; i++)
	{
		std::string uniformName = "materials[";
		uniformName.append(std::to_string(i));
		uniformName.append("].");

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
		//AddUniform(uniformName + OCCLUSION);
	}

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string uniformName = "lights[";
		uniformName.append(std::to_string(i));
		uniformName.append("]");

		AddUniform(uniformName + ".position");
		AddUniform(uniformName + ".colour");
		AddUniform(uniformName + ".intensity");
	}

	AddUniform("highDetailRange");
	AddUniform("gamma");
	AddUniform("numberOfLights");

	//AddUniform("irradianceMap");
	//AddUniform("prefilterMap");
	//AddUniform("brdfLUT");
}

void GLTerrainShader::UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const
{
	TerrainNode* node = (TerrainNode*)object;

	node->getMaps()->getHeightmap()->Bind(0);
	SetUniformi("heightmap", 0);
	/*node->getMaps()->getNormalmap()->Bind(1);
	SetUniformi("normalmap", 0);
	node->getMaps()->getSplatmap()->Bind(2);
	SetUniformi("splatmap", 0);*/

	SetUniform("localMatrix", object->GetLocalTransform()->getTransformationMatrix());
	SetUniform("worldMatrix", object->GetWorldTransform()->getTransformationMatrix());

	SetUniform("cameraPosition", camera->getPosition());
	SetUniform("location", node->getLocation());
	SetUniform("index", node->getIndex());
	SetUniformf("scaleY", TerrainConfig::scaleY);
	SetUniformf("gap", node->getGap());
	SetUniformi("lod", node->getLod());

	for (unsigned int i = 0; i < 8; i++)
	{
		SetUniformi("lodMorphArea[" + std::to_string(i) + "]", TerrainConfig::lodMorphingAreas[i]);
	}

	SetUniformi("tessellationFactor", TerrainConfig::tessellationFactor);
	SetUniformf("tessellationSlope", TerrainConfig::tessellationSlope);
	SetUniformf("tessellationShift", TerrainConfig::tessellationShift);

	SetUniform("viewProjection", camera->getViewProjectionMatrix());
	
	uint32_t texUnit = 3;

	for (unsigned int i = 0; i < 3; i++)
	{
		std::string uName = "materials[" + std::to_string(i) + "].";

		TerrainConfig::materials[i]->GetTexture(ALBEDO_MAP)->Bind(texUnit);
		SetUniformi(uName + ALBEDO_MAP, texUnit);
		texUnit++;

		TerrainConfig::materials[i]->GetTexture(NORMAL_MAP)->Bind(texUnit);
		SetUniformi(uName + NORMAL_MAP, texUnit);
		texUnit++;

		TerrainConfig::materials[i]->GetTexture(DISPLACEMENT_MAP)->Bind(texUnit);
		SetUniformi(uName + DISPLACEMENT_MAP, texUnit);
		texUnit++;

		TerrainConfig::materials[i]->GetTexture(METALLIC_MAP)->Bind(texUnit);
		SetUniformi(uName + METALLIC_MAP, texUnit);
		texUnit++;

		TerrainConfig::materials[i]->GetTexture(ROUGHNESS_MAP)->Bind(texUnit);
		SetUniformi(uName + ROUGHNESS_MAP, texUnit);
		texUnit++;

		SetUniformf(uName + HEIGHT_SCALE, TerrainConfig::materials[i]->GetFloat(HEIGHT_SCALE));
		SetUniformf(uName + HORIZONTAL_SCALE, TerrainConfig::materials[i]->GetFloat(HORIZONTAL_SCALE));
		SetUniformf(uName + METALLIC, TerrainConfig::materials[i]->GetFloat(METALLIC));
		SetUniformf(uName + ROUGHNESS, TerrainConfig::materials[i]->GetFloat(ROUGHNESS));
		//SetUniformf(uName + OCCLUSION, TerrainConfig::materials[i]->GetFloat(OCCLUSION));
	}

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::string uName = "lights[" + std::to_string(i) + "].";

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform(uName + "position", light->GetParent()->GetWorldTransform()->GetPosition());
			SetUniform(uName + "colour", light->GetColour());
			SetUniform(uName + "intensity", light->GetIntensity());
		}
		else
		{
			SetUniform(uName + "position", Vector3f());
			SetUniform(uName + "colour", Vector3f());
			SetUniform(uName + "intensity", Vector3f());
		}
	}

	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformi("numberOfLights", (int) lights.size());
}