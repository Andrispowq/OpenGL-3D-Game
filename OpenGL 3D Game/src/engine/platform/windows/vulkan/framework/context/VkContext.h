#ifndef VK_CONTEXT_H
#define VK_CONTEXT_H

#include <vector>

#include "VkDebugMessenger.h"
#include "engine/prehistoric/common/framework/context/Context.h"

class VkContext : public Context
{
public:
	bool InitContext() override;
	bool DeleteContext() override;
private:
	bool CheckValidationLayerSupport() const;
	std::vector<const char*> GetRequiredExtensions() const;
private:
	VkInstance instance;
	VkDebugMessenger messenger;

	std::vector<const char*> validationLayers;
	bool useValidationLayers;
};

#endif