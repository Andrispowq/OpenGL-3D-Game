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
		std::stringstream uName;
		uName << "lodMorphArea[";
		uName << i;
		uName << "]";

		AddUniform(uName.str());
	}

	AddUniform("tessellationFactor");
	AddUniform("tessellationSlope");
	AddUniform("tessellationShift");

	AddUniform("viewProjection");

	AddUniform("normalmap");
	AddUniform("splatmap");

	for (unsigned int i = 0; i < 3; i++)
	{
		std::stringstream uniformName;
		uniformName << "materials[";
		uniformName << i;
		uniformName << "].";

		AddUniform(uniformName.str() + ALBEDO_MAP);
		AddUniform(uniformName.str() + NORMAL_MAP);
		AddUniform(uniformName.str() + DISPLACEMENT_MAP);
		AddUniform(uniformName.str() + METALLIC_MAP);
		AddUniform(uniformName.str() + ROUGHNESS_MAP);
		//AddUniform(uniformName.str() + OCCLUSION_MAP);

		AddUniform(uniformName.str() + HEIGHT_SCALE);
		AddUniform(uniformName.str() + HORIZONTAL_SCALE);
		AddUniform(uniformName.str() + METALLIC);
		AddUniform(uniformName.str() + ROUGHNESS);
		//AddUniform(uniformName.str() + OCCLUSION);
	}

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::stringstream uniformName;
		uniformName << "lights[";
		uniformName << i;
		uniformName << "].";

		AddUniform(uniformName.str() + "position");
		AddUniform(uniformName.str() + "colour");
		AddUniform(uniformName.str() + "intensity");
	}

	AddUniform("highDetailRange");
	AddUniform("gamma");
	AddUniform("numberOfLights");

	//AddUniform("irradianceMap");
	//AddUniform("prefilterMap");
	//AddUniform("brdfLUT");
}

void GLTerrainShader::UpdateShaderUniforms(Camera* camera, std::vector<Light*> lights) const
{
	SetUniform("viewProjection", camera->getViewProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());

	for (unsigned int i = 0; i < EngineConfig::lightsMaxNumber; i++)
	{
		std::stringstream uniformName;
		uniformName << "lights[";
		uniformName << i;
		uniformName << "].";

		if (i < lights.size())
		{
			Light* light = lights[i];

			SetUniform(uniformName.str() + "position", light->GetParent()->GetWorldTransform()->GetPosition());
			SetUniform(uniformName.str() + "colour", light->GetColour());
			SetUniform(uniformName.str() + "intensity", light->GetIntensity());
		}
		else
		{
			SetUniform(uniformName.str() + "position", Vector3f());
			SetUniform(uniformName.str() + "colour", Vector3f());
			SetUniform(uniformName.str() + "intensity", Vector3f());
		}
	}

	SetUniformi("highDetailRange", EngineConfig::rendererHighDetailRange);
	SetUniformf("gamma", EngineConfig::rendererGamma);
	SetUniformi("numberOfLights", (uint32_t)lights.size() < EngineConfig::lightsMaxNumber ? (uint32_t)lights.size() : EngineConfig::lightsMaxNumber);
}

void GLTerrainShader::UpdateSharedUniforms(GameObject* object) const
{
	TerrainNode* node = (TerrainNode*)object;

	node->getMaps()->getHeightmap()->Bind(0);
	SetUniformi("heightmap", 0);
	node->getMaps()->getNormalmap()->Bind(1);
	SetUniformi("normalmap", 1);
	//node->getMaps()->getSplatmap()->Bind(2);
	//SetUniformi("splatmap", 0);

	SetUniformf("scaleY", TerrainConfig::scaleY);

	SetUniformi("tessellationFactor", TerrainConfig::tessellationFactor);
	SetUniformf("tessellationSlope", TerrainConfig::tessellationSlope);
	SetUniformf("tessellationShift", TerrainConfig::tessellationShift);

	uint32_t texUnit = 3;

	for (unsigned int i = 0; i < 3; i++)
	{
		std::stringstream uniformName;
		uniformName << "materials[";
		uniformName << i;
		uniformName << "].";

		Material* material = TerrainConfig::materials[i];

		material->GetTexture(ALBEDO_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + ALBEDO_MAP, texUnit);
		texUnit++;

		material->GetTexture(NORMAL_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + NORMAL_MAP, texUnit);
		texUnit++;

		material->GetTexture(DISPLACEMENT_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + DISPLACEMENT_MAP, texUnit);
		texUnit++;

		material->GetTexture(METALLIC_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + METALLIC_MAP, texUnit);
		texUnit++;

		material->GetTexture(ROUGHNESS_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + ROUGHNESS_MAP, texUnit);
		texUnit++;

		SetUniformf(uniformName.str() + HEIGHT_SCALE, material->GetFloat(HEIGHT_SCALE));
		SetUniformf(uniformName.str() + HORIZONTAL_SCALE, material->GetFloat(HORIZONTAL_SCALE));
		SetUniformf(uniformName.str() + METALLIC, material->GetFloat(METALLIC));
		SetUniformf(uniformName.str() + ROUGHNESS, material->GetFloat(ROUGHNESS));
		//SetUniformf(uniformName.str() + OCCLUSION, material->GetFloat(OCCLUSION));
	}
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

	for (unsigned int i = 0; i < 8; i++)
	{
		std::stringstream uName;
		uName << "lodMorphArea[";
		uName << i;
		uName << "]";

		SetUniformi(uName.str(), TerrainConfig::lodMorphingAreas[i]);
	}
}