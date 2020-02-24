#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "engine/core/movement/Camera.h"

class Transform
{
public:
	Transform(const Vector3f& position, const Vector3f& rotation, const Vector3f& scaling)
		: position(position), rotation(rotation), scaling(scaling) {}
	Transform() : position(Vector3f()), rotation(Vector3f()), scaling(Vector3f(1)) {}

	virtual ~Transform() {}

	Matrix4f getTransformationMatrix() const;
	Matrix4f getModelViewProjectionMatrix(const Camera& camera) const;

	static Matrix4f getTransformationMatrix(const Vector3f& position, const Vector3f& rotation = Vector3f(), const Vector3f& scaling = Vector3f(1));

	inline Vector3f GetPosition() const { return position; }
	inline Vector3f GetRotation() const { return rotation; }
	inline Vector3f GetScaling() const { return scaling; }

	inline void SetPosition(const Vector3f& position) { this->position = position; }
	inline void SetRotation(const Vector3f& rotation) { this->rotation = rotation; }
	inline void SetScaling(const Vector3f& scaling) { this->scaling = scaling; }
private:
	Vector3f position;
	Vector3f rotation;
	Vector3f scaling;
};

#endif