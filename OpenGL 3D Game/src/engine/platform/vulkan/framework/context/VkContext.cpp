#include "engine/prehistoric/core/util/Includes.hpp"
#include "VkContext.h"

static void ListExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	PR_LOG_MESSAGE("Available extensions:\n");

	for (const auto& extension : extensions)
	{
		PR_LOG_MESSAGE("\t%s\n", extension.extensionName);
	}
}

//Context creation and other related stuff
bool VkContext::InitContext(Window* window)
{
	if (FrameworkConfig::apiVulkanUseValidationLayers)
	{
		validationLayers.push_back("VK_LAYER_KHRONOS_validation");

		if (!CheckValidationLayerSupport())
		{
			PR_LOG_RUNTIME_ERROR("Validation layers were requested, but they are not available!");
		}
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = FrameworkConfig::windowName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(FrameworkConfig::apiVersion.x, FrameworkConfig::apiVersion.y, 0);
	appInfo.pEngineName = FrameworkConfig::windowName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(FrameworkConfig::apiVersion.x, FrameworkConfig::apiVersion.y, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	PR_LOG_MESSAGE("%i\n", appInfo.apiVersion);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Extensions
	auto extensions = GetRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT createMessenger;
	if (FrameworkConfig::apiVulkanUseValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugMessenger::CreateMessengerCreateInfo(createMessenger);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &createMessenger;
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
	{
		PR_LOG_RUNTIME_ERROR("Failed to create vulkan instance!");
		return false;
	}

	//ListExtensions();

	if (FrameworkConfig::apiVulkanUseValidationLayers)
	{
		messenger.CreateMessenger(instance);
	}

	if (glfwCreateWindowSurface(instance, reinterpret_cast<GLFWwindow*>(window->GetWindowHandle()), nullptr, &surface) != VK_SUCCESS)
	{
		PR_LOG_RUNTIME_ERROR("Failed to create window surface!");
	}

	reinterpret_cast<VkSwapChain*>(window->GetSwapChain())->SetSurface(&surface);

	physicalDeviceSelector.PickPhysicalDevice(surface, instance);
	logicalDeviceSelector.CreateLogicalDevice(surface, physicalDeviceSelector.GetPhysicalDevice(), validationLayers, physicalDeviceSelector.GetDeviceExtensions());

	return true;
}

bool VkContext::DeleteContext(Window* window)
{
	window->DeleteSwapChain(&(logicalDeviceSelector.GetDevice()));

	logicalDeviceSelector.DestroyLogicalDevice();

	if (FrameworkConfig::apiVulkanUseValidationLayers)
	{
		messenger.DeleteMessenger(instance);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
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
}

std::vector<const char*> VkContext::GetRequiredExtensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (FrameworkConfig::apiVulkanUseValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}