#include "engine/prehistoric/core/util/Includes.hpp"
#include "EnviromentMapRenderer.h"

#include "engine/config/EnviromentMapConfig.h"

#include "engine/platform/opengl/texture/GLTexture.h"

#include "engine/platform/opengl/rendering/framebuffer/GLFramebuffer.h"

EnviromentMapRenderer::EnviromentMapRenderer(Window* window, AssembledAssetManager* manager)
	: window(window), manager(manager)
{
	projectionMatrix = Matrix4f::PerspectiveProjection(90, 1, .1f, 100.0f);

	viewMatrices[0] = Matrix4f::View(Vector3f(1, 0, 0), Vector3f(0, -1, 0));
	viewMatrices[1] = Matrix4f::View(Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
	viewMatrices[2] = Matrix4f::View(Vector3f(0, 1, 0), Vector3f(0, 0, -1));
	viewMatrices[3] = Matrix4f::View(Vector3f(0, -1, 0), Vector3f(0, 0, 1));
	viewMatrices[4] = Matrix4f::View(Vector3f(0, 0, -1), Vector3f(0, -1, 0));
	viewMatrices[5] = Matrix4f::View(Vector3f(0, 0, 1), Vector3f(0, -1, 0));

	equirectangularMap = TextureLoader::LoadTexture(EnviromentMapConfig::enviromentMapLocation, window);
	equirectangularMapID = manager->getAssetManager()->addResource<Texture>(equirectangularMap);

	framebuffer = new GLFramebuffer(window);
}

EnviromentMapRenderer::~EnviromentMapRenderer()
{
}

void EnviromentMapRenderer::GenerateEnviromentMap(Engine* engine)
{
}
