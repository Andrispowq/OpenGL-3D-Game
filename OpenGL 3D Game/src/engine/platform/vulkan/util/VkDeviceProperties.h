#ifndef VK_DEVICE_PROPERTIES
#define VK_DEVICE_PROPERTIES

#include "vulkan/vulkan.h"

#include "engine/prehistoric/common/util/DeviceProperties.h"

class VKDeviceProperties : public DeviceProperties
{
public:
	virtual void ListProperties(void* device) const override;
};

#endif