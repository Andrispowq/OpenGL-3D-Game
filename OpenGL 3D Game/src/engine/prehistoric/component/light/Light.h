#ifndef LIGHT_H
#define LIGHT_H

#include "engine/prehistoric/component/Component.h"

class Light : public Component
{
public:
	Light(Vector3f colour = 0, Vector3f intensity = 1) : Component(), colour(colour), intensity(intensity) {}
	virtual ~Light() {}

	virtual void PreRender(RenderingEngine* renderingEngine) override;

	inline Vector3f GetColour() const { return colour; }
	inline Vector3f GetIntensity() const { return intensity; }

	inline void SetColour(const Vector3f& colour) { this->colour = colour; }
	inline void SetIntensity(const Vector3f& intensity) { this->intensity = intensity; }
private:
	Vector3f colour;
	Vector3f intensity;
};

#endif