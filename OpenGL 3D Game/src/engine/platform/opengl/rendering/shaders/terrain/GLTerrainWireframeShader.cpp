#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLTerrainWireframeShader.h"

#include "engine/prehistoric/modules/terrain/TerrainNode.h"

GLTerrainWireframeShader::GLTerrainWireframeShader()
{
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_VS.glsl"), VERTEX_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_TC.glsl"), TESSELLATION_CONTROL_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_TE.glsl"), TESSELLATION_EVALUATION_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_wGS.glsl"), GEOMETRY_SHADER);
	AddShader(ResourceLoader::LoadShaderGL("opengl/terrain/terrain_wFS.glsl"), FRAGMENT_SHADER);
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

	AddUniform("splatmap");

	for (unsigned int i = 0; i < 3; i++)
	{
		std::string uniformName = "materials[";
		uniformName.append(std::to_string(i));
		uniformName.append("].");

		AddUniform(uniformName + DISPLACEMENT_MAP);

		AddUniform(uniformName + HEIGHT_SCALE);
		AddUniform(uniformName + HORIZONTAL_SCALE);
	}
}

void GLTerrainWireframeShader::UpdateUniforms(GameObject* object, Camera* camera, std::vector<Light*> lights) const
{
	TerrainNode* node = (TerrainNode*)object;

	node->getMaps()->getHeightmap()->Bind(0);
	SetUniformi("heightmap", 0);
	/*node->getMaps()->getSplatmap()->Bind(2);
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

		TerrainConfig::materials[i]->GetTexture(DISPLACEMENT_MAP)->Bind(texUnit);
		SetUniformi(uName + DISPLACEMENT_MAP, texUnit);
		texUnit++;

		SetUniformf(uName + HEIGHT_SCALE, TerrainConfig::materials[i]->GetFloat(HEIGHT_SCALE));
		SetUniformf(uName + HORIZONTAL_SCALE, TerrainConfig::materials[i]->GetFloat(HORIZONTAL_SCALE));
	}
}