#ifndef MODEL_FABRICATOR_H
#define MODEL_FABRICATOR_H

#include "engine/prehistoric/common/buffer/MeshVBO.h"
#include "engine/prehistoric/common/framework/Window.h"

#include "engine/config/FrameworkConfig.h"

namespace ModelFabricator
{
	MeshVBO* CreateQuad(Window* window);
};

#endif