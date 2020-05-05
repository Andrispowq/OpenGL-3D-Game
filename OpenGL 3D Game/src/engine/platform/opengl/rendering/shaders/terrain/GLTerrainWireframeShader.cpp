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

	AddUniform("splatmap");

	for (unsigned int i = 0; i < 3; i++)
	{
		std::stringstream uniformName;
		uniformName << "materials[";
		uniformName << i;
		uniformName << "].";

		AddUniform(uniformName.str() + DISPLACEMENT_MAP);

		AddUniform(uniformName.str() + HEIGHT_SCALE);
		AddUniform(uniformName.str() + HORIZONTAL_SCALE);
	}
}

void GLTerrainWireframeShader::UpdateShaderUniforms(Camera* camera, std::vector<Light*> lights) const
{
	SetUniform("viewProjection", camera->getViewProjectionMatrix());
	SetUniform("cameraPosition", camera->getPosition());
}

void GLTerrainWireframeShader::UpdateSharedUniforms(GameObject* object) const
{
	TerrainNode* node = (TerrainNode*)object;

	node->getMaps()->getHeightmap()->Bind(0);
	SetUniformi("heightmap", 0);
	/*node->getMaps()->getSplatmap()->Bind(1);
	SetUniformi("splatmap", 1);*/

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

		material->GetTexture(DISPLACEMENT_MAP)->Bind(texUnit);
		SetUniformi(uniformName.str() + DISPLACEMENT_MAP, texUnit);
		texUnit++;

		SetUniformf(uniformName.str() + HEIGHT_SCALE, material->GetFloat(HEIGHT_SCALE));
		SetUniformf(uniformName.str() + HORIZONTAL_SCALE, material->GetFloat(HORIZONTAL_SCALE));
	}
}

void GLTerrainWireframeShader::UpdateObjectUniforms(GameObject* object) const
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