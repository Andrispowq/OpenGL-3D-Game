#ifndef LIGHT_H
#define LIGHT_H

#include "engine/prehistoric/component/Component.h"

class Light : public Component
{
public:
	Light(const Vector3f& colour = 0, const Vector3f& intensity = 1, bool isSun = false) : Component(), colour(colour), intensity(intensity), isSun(isSun) {}
	virtual ~Light() {}

	virtual void PreRender(RenderingEngine* renderingEngine) override;

	inline Vector3f GetColour() const { return colour; }
	inline Vector3f GetIntensity() const { return intensity; }

	inline bool IsSun() const { return isSun; }

	inline void SetColour(const Vector3f& colour) { this->colour = colour; }
	inline void SetIntensity(const Vector3f& intensity) { this->intensity = intensity; }

	inline void SetSun(bool sun) { this->isSun = sun; }
private:
	Vector3f colour;
	Vector3f intensity;

	bool isSun;
};

#endif