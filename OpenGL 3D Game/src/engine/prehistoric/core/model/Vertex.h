#ifndef VERTEX_H
#define VERTEX_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/prehistoric/core/math/Math.h"

class Vertex
{
public:
	Vertex(const Vector3f& position, const Vector2f& texture, const Vector3f& normal) : position(position), texture(texture), normal(normal), textureIndex(-1), normalIndex(-1) {}
	Vertex(const Vector3f& position, const Vector2f& texture) : position(position), texture(texture), normal(Vector3f(0)), textureIndex(-1), normalIndex(-1) {}
	Vertex(const Vector3f& position) : position(position), texture(Vector2f(0)), normal(Vector3f(0)), textureIndex(-1), normalIndex(-1) {}
	Vertex() : position(Vector3f(0)), texture(Vector2f(0)), normal(Vector3f(0)), textureIndex(-1), normalIndex(-1) {}

	Vertex(uint16_t index, Vector3f position) : index(index), position(position), length(position.length()), textureIndex(-1), normalIndex(-1) {}

	~Vertex() { /*delete duplicateVertex;*/ }

	constexpr static size_t getNumberOfFloats() { return 8; }
	constexpr static size_t getSize() { return getNumberOfFloats() * sizeof(float); }

	inline Vector3f getPosition() const { return position; }
	inline Vector2f getTexture() const { return texture; }
	inline Vector3f getNormal() const { return normal; }

	inline int getTextureIndex() const { return textureIndex; }
	inline int getNormalIndex() const { return normalIndex; }

	inline bool isSet() const { return textureIndex != -1 && normalIndex != -1; }
	inline bool hasSameTextureAndNormal(int textureIndex, int normalIndex) const { return this->textureIndex == textureIndex && this->normalIndex == normalIndex; }

	inline Vertex* getDuplicateVertex() const { return duplicateVertex; }

	inline uint16_t getIndex() const { return index; }

	inline void setPosition(const Vector3f& position) { this->position = position; }
	inline void setTexture(const Vector2f& texture) { this->texture = texture; }
	inline void setNormal(const Vector3f& normal) { this->normal = normal; }

	inline void setTextureIndex(int textureIndex) { this->textureIndex = textureIndex; }
	inline void setNormalIndex(int normalIndex) { this->normalIndex = normalIndex; }

	inline void setDuplicateVertex(Vertex* vertex) { this->duplicateVertex = vertex; }

	inline void setIndex(int index) { this->index = index; }
public:
	Vector3f position;
	Vector2f texture;
	Vector3f normal;

	//Data for loading obj files
	Vertex* duplicateVertex = nullptr;

	int textureIndex;
	int normalIndex;
	uint16_t index = -1;

	float length = 0;
};

#endif