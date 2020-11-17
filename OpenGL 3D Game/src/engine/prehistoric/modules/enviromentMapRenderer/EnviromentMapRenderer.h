#ifndef ENVIROMENT_RENDERER_H
#define ENVIROMENT_RENDERER_H

#include "engine/prehistoric/common/rendering/framebuffer/Framebuffer.h"
#include "engine/prehistoric/common/texture/Texture.h"
#include "engine/prehistoric/common/buffer/MeshVertexBuffer.h"

#include "engine/prehistoric/resources/AssembledAssetManager.h"

class EnviromentMapRenderer
{
public:
	EnviromentMapRenderer(Window* window, AssembledAssetManager* manager);
	virtual ~EnviromentMapRenderer();

	void GenerateEnviromentMap(Engine* engine);

	Texture* getEnviromentMap() const { return enviromentMap; }
	Texture* getIrradianceMap() const { return irradianceMap; }
	Texture* getPrefilterMap() const { return prefilterMap; }
	Texture* getBRDFMap() const { return brdfMap; }

private:
	Window* window;
	AssembledAssetManager* manager;

	Framebuffer* framebuffer;

	Texture* equirectangularMap;
	Texture* enviromentMap;
	Texture* irradianceMap;
	Texture* prefilterMap;
	Texture* brdfMap;

	Pipeline* enviromentMapPipeline;
	Pipeline* irradiancePipeline;
	Pipeline* prefilterPipeline;
	Pipeline* brdfIntegratePipeline;
	Pipeline* backgroundPipeline;

	Matrix4f viewMatrices[6];
	Matrix4f projectionMatrix;

	//Just so that we have the handles to the resources in the manager:
	size_t equirectangularMapID;
	size_t enviromentMapID;
	size_t irradianceMapID;
	size_t prefilterMapID;
	size_t brdfMapID;

	size_t enviromentMapPipelineID;
	size_t irradiancePipelineID;
	size_t prefilterPipelineID;
	size_t brdfIntegratePipelineID;
	size_t backgroundPipelineID;
};

#endif