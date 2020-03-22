#include "engine/prehistoric/core/util/Includes.hpp"
#include "ModelFabricator.h"
#include "engine/platform/opengl/buffer/GLMeshVBO.h"

namespace ModelFabricator
{
	MeshVBO* ModelFabricator::CreateQuad()
	{
		MeshVBO* vbo = nullptr;

		if (FrameworkConfig::api == OpenGL)
		{
			vbo = new GLMeshVBO();
		}
		else if (FrameworkConfig::api == Vulkan)
		{
			PR_LOG_RUNTIME_ERROR("Vulkan API does not currently support VBOs! Switch to OpenGL (version 3.3 at least)!");
		}

		Mesh mesh;

		mesh.AddVertex(Vertex(Vector3f(-1, -1, 0), Vector2f(0, 0), Vector3f(0, 0, -1)));
		mesh.AddVertex(Vertex(Vector3f(-1,  1, 0), Vector2f(0, 1), Vector3f(0, 0, -1)));
		mesh.AddVertex(Vertex(Vector3f( 1,  1, 0), Vector2f(1, 1), Vector3f(0, 0, -1)));
		mesh.AddVertex(Vertex(Vector3f( 1, -1, 0), Vector2f(1, 0), Vector3f(0, 0, -1)));

		mesh.setIndices({ 0, 1, 2, 2, 3, 0 });

		vbo->Store(mesh);

		return vbo;
	}
};