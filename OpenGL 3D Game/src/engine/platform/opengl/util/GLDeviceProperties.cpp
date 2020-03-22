#include "engine/prehistoric/core/util/Includes.hpp"
#include "GLDeviceProperties.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

void GLDeviceProperties::ListProperties(const void* device) const
{
	PR_LOG_MESSAGE("Video card: %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	PR_LOG_MESSAGE("Manufacturer: %s\n\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

	PR_LOG_MESSAGE("Other important informations: \n");

	GLint total_vram_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_vram_kb);
	PR_LOG_MESSAGE("\tAmount of VRAM: %i MB, %i GB\n", total_vram_kb / 1024, total_vram_kb / (1024 * 1024));

	GLint max_texture_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
	PR_LOG_MESSAGE("\tMaximum size of textures: %iK\n", (max_texture_size / 1000));

	GLint max_number_of_textures;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_number_of_textures);
	PR_LOG_MESSAGE("\tMaximum number of textures: %i\n\n", max_number_of_textures);
}