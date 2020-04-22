#ifndef VK_PBR_SHADER_H
#define VK_PBR_SHADER_H

#include "engine/platform/vulkan/rendering/shaders/VkShader.h"

#include <vulkan/vulkan.h>

#include "engine/prehistoric/core/gameObject/GameObject.h"
#include "engine/prehistoric/core/model/material/Material.h"
#include "engine/prehistoric/component/light/Light.h"
#include "engine/prehistoric/component/renderer/Renderer.h"

class VKPBRShader : public VKShader
{
public:
	VKPBRShader(Window* window);
	virtual ~VKPBRShader() {}

	virtual void UpdateUniforms(GameObject* root, Camera* camera, std::vector<Light*> lights) const override;
};

#endif