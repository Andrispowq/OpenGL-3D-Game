#include "engine/prehistoric/core/util/Includes.hpp"
#include "TerrainHeightsQuery.h"

TerrainHeightsQuery::TerrainHeightsQuery(Window* window, uint32_t N)
{
	this->window = window;

	this->N = N;

	//TODO: VK PIPELINE
	if (FrameworkConfig::api == OpenGL)
	{
		pipeline = new GLComputePipeline(new GLTerrainHeightsShader());
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//pipeline = new VKComputePipeline(new VKTerrainHeightsShader());
	}

	pipeline->SetViewportStart({ 0, 0 });
	pipeline->SetViewportSize({ (float)FrameworkConfig::windowWidth, (float)FrameworkConfig::windowHeight });
	pipeline->SetScissorStart({ 0, 0 });
	pipeline->SetScissorSize({ FrameworkConfig::windowWidth, FrameworkConfig::windowHeight });

	pipeline->CreatePipeline(window);

	//TODO: TEXTURES
	//heights = new float[N * N];

	if (FrameworkConfig::api == OpenGL)
	{
		buffer = new GLShaderStorageBuffer();
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//buffer = new VKShaderStorageBuffer();
	}

	Layout layout;
	layout.addLayoutMember(LayoutType::FLOAT, LayoutTypeInfo::UNSIZED_ARRAY, N * N);
	/*layout.memberCount = 1;
	layout.members = new LayoutMember();
	
	layout.members[0].count = N * N;
	layout.members[0].type = LayoutType::FLOAT;
	layout.members[0].typeInfo = LayoutTypeInfo::SINGLE_ELEMENT;*/

	buffer->Store(heights, layout);

	if (FrameworkConfig::api == OpenGL)
	{
		GLComputePipeline* glPipe = reinterpret_cast<GLComputePipeline*>(pipeline);
		glPipe->setInvocationSize({ N / 16, N / 16, 1 });
		glPipe->addSSBOBinding(0, buffer, WRITE_ONLY);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		/*VKComputePipeline* vkPipe = reinterpret_cast<VKComputePipeline*>(pipeline);
		vkPipe->setInvocationSize({ float(N / 16), float(N / 16), 1.0f });
		vkPipe->addTextureBinding(0, buffer, WRITE_ONLY);*/
	}
}

TerrainHeightsQuery::~TerrainHeightsQuery()
{
	pipeline->DestroyPipeline();

	delete pipeline;
	delete buffer; //This deletes the mapping to heights*
}

void TerrainHeightsQuery::Query(Texture* heightmap)
{
	pipeline->BindPipeline();

	if (FrameworkConfig::api == OpenGL)
	{
		reinterpret_cast<GLTerrainHeightsShader*>(pipeline->getShader())->UpdateUniforms(heightmap, N);
	}
	else if (FrameworkConfig::api == Vulkan)
	{
		//reinterpret_cast<VKTerrainHeightsShader*>(pipeline->getShader())->UpdateUnifroms(heightmap, N);
	}

	pipeline->RenderPipeline();
	pipeline->UnbindPipeline();

	buffer->Bind(nullptr, 0);
	buffer->MapBuffer();
	heights = (float*) buffer->getMappedData();
	buffer->UnmapBuffer();
	buffer->Unbind();
}
