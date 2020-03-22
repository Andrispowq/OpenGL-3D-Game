#ifndef FRAMEWORKCONFIG_H
#define FRAMEWORKCONFIG_H

#include <fstream>

#include "engine/prehistoric/core/util/Util.h"
#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/log/Log.h"

/*
	Implemented:
		OpenGL
	Under development:
		Vulkan
	Planned:
		Vulkan_RTX, DirectX_11, DirectX_12, DirectX_RTX
*/
enum API
{
	OpenGL, Vulkan, Vulkan_RTX, DirectX_11, DirectX_12, DirectX_RTX, NO_API
};

namespace FrameworkConfig
{
	void LoadConfig(const std::string& path);

	extern std::string windowName;
	extern unsigned int windowWidth;
	extern unsigned int windowHeight;
	extern bool windowFullScreen;
	extern bool windowResizable;
	extern bool windowVSync;
	extern unsigned int windowNumSamples;
	extern unsigned int windowMaxFPS;

	extern API api;
	extern Vector2i apiVersion;
	extern bool apiVulkanUseValidationLayers;
};

#endif