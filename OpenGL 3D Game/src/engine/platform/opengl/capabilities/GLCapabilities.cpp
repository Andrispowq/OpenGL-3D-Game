#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLCapabilities.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

void GLCapabilities::QueryCapabilities(void* physicalDevice)
{
	physicalDeviceCaps.name = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

	std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	std::vector<std::string> versions = Util::Split(version, '.');
	driverCaps.apiVersionMajor = std::atoi(versions[0].c_str());
	driverCaps.apiVersionMinor = std::atoi(versions[1].c_str());
	driverCaps.apiVersionPatch = std::atoi(versions[2].c_str());

	int32_t size;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &size);
	memoryCaps.vramSize = size;

	int32_t maxTexSlots;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTexSlots);
	limits.maxTextureSlots = maxTexSlots;

	int32_t maxTexSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	limits.maxTextureResolution = maxTexSize;
}