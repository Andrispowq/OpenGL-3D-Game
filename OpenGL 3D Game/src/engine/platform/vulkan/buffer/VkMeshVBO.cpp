#include "engine/prehistoric/core/util/Includes.hpp"
#include "VKMeshVBO.h"

VKMeshVBO::VKMeshVBO(const Mesh& mesh)
{

}

VKMeshVBO::~VKMeshVBO()
{

}

void VKMeshVBO::Store(const Mesh& mesh)
{

}

void VKMeshVBO::Bind() const
{

}

void VKMeshVBO::Draw() const
{
	vkCmdDraw(buffer, 3, 1, 0, 0);
}

void VKMeshVBO::Unbind() const
{

}