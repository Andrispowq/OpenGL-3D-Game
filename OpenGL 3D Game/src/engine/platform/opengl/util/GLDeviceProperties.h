#ifndef GL_DEVICE_PROPERTIES
#define GL_DEVICE_PROPERTIES

#include "glew.h"

#include "engine/prehistoric/common/util/DeviceProperties.h"

class GLDeviceProperties : public DeviceProperties
{
public:
	virtual void ListProperties(const void* device) const override;
};

#endif