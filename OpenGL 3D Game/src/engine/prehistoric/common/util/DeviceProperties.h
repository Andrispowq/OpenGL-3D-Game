#ifndef DEVICE_PROPERTIES
#define DEVICE_PROPERTIES

class DeviceProperties
{
public:
	static DeviceProperties& GetInstance();
	static void DeleteInstance();

	virtual void ListProperties(void* device) const = 0;
protected:
	DeviceProperties() {}
	virtual ~DeviceProperties() {}
private:
	static DeviceProperties* instance;
};

#endif