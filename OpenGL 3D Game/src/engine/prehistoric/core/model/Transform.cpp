#include "engine/prehistoric/core/util/Includes.hpp"
#include "Transform.h"

Matrix4f Transform::getTransformationMatrix() const
{
	return Matrix4f::Transformation(position, rotation, scaling);
}

Matrix4f Transform::getModelViewProjectionMatrix(const Camera& camera) const
{
	return camera.GetViewProjectionMatrix() * getTransformationMatrix();
}

Matrix4f Transform::getTransformationMatrix(const Vector3f& position, const Vector3f& rotation, const Vector3f& scaling)
{
	return Matrix4f::Transformation(position, rotation, scaling);
}