#include "VkContext.h"
#include "engine/config/FrameworkConfig.h"
#include "glfw3.h"

//Context creation and other related stuff
bool VkContext::InitContext()
{
	if (useValidationLayers && !CheckValidationLayerSupport()) 
	{
		throw std::runtime_error("Validation layers were requested, but they are not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = FrameworkConfig::windowName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(FrameworkConfig::apiVersion.x, FrameworkConfig::apiVersion.y, 0);
	appInfo.pEngineName = FrameworkConfig::windowName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(FrameworkConfig::apiVersion.x, FrameworkConfig::apiVersion.y, 0);
	appInfo.apiVersion = VK_MAKE_VERSION(FrameworkConfig::apiVersion.x, FrameworkConfig::apiVersion.y, 0);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Extensions
	auto extensions = GetRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (useValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to create vulkan instance!");
		return false;
	}

	//ListExtensions();

	messenger.CreateMessenger(instance);

	return true;
}

static void ListExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions:" << std::endl;

	for (const auto& extension : extensions)
	{
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

bool VkContext::DeleteContext()
{
	if (useValidationLayers)
	{
		messenger.DeleteMessenger(instance);
	}

	vkDestroyInstance(instance, nullptr);

	return true;
}

bool VkContext::CheckValidationLayerSupport() const
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) 
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) 
		{
			if (strcmp(layerName, layerProperties.layerName) == 0) 
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;

	return false;
}

std::vector<const char*> VkContext::GetRequiredExtensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (useValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}