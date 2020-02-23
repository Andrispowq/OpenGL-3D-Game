#ifndef VERTEX_H
#define VERTEX_H

#include "engine/core/math/Math.h"

class Vertex
{
public:
	Vertex(const Vector3f& position, const Vector2f& texture, const Vector3f& normal) : position(position), texture(texture), normal(normal) {}
	Vertex(const Vector3f& position) : position(position), texture(Vector2f(0)), normal(Vector3f(0)) {}
	Vertex() : position(Vector3f(0)), texture(Vector2f(0)), normal(Vector3f(0)) {}

	virtual ~Vertex() {}

	inline Vector3f getPosition() const { return position; }
	inline Vector2f getTexture() const { return texture; }
	inline Vector3f getNormal() const { return normal; }

	inline void setPosition(const Vector3f& position) { this->position = position; }
	inline void setTexture(const Vector2f& texture) { this->texture = texture; }
	inline void setNormal(const Vector3f& normal) { this->normal = normal; }
private:
	Vector3f position;
	Vector2f texture;
	Vector3f normal;
};

#endif