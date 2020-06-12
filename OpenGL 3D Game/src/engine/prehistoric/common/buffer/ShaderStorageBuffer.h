#ifndef SSBO_H
#define SSBO_H

#include "engine/prehistoric/core/math/Math.h"

enum class LayoutType
{
	UNSIGNED_INT,
	SIGNED_INT,
	FLOAT,
	DOUBLE,
	VECTOR2F,
	VECTOR3F,
	VECTOR4F
};

enum class LayoutTypeInfo
{
	SINGLE_ELEMENT,
	SIZED_ARRAY,
	UNSIZED_ARRAY
};

struct LayoutMember
{
	LayoutType type;
	LayoutTypeInfo typeInfo;
	size_t count;
};

struct Layout
{
	constexpr static size_t MAX_MEMBERS = 10;

	LayoutMember* members = nullptr;
	size_t memberCount = 0;

	void addLayoutMember(LayoutType type, LayoutTypeInfo typeInfo, size_t count)
	{
		if (members == nullptr)
		{
			members = new LayoutMember[MAX_MEMBERS];
		}

		members[memberCount].count = count;
		members[memberCount].type = type;
		members[memberCount].typeInfo = typeInfo;

		memberCount++;
	}

	size_t getSize() const
	{
		size_t size = 0;

		for (size_t i = 0; i < memberCount; i++)
		{
			LayoutMember member = members[i];

			switch (member.type)
			{
			case LayoutType::UNSIGNED_INT:
				size += sizeof(uint32_t) * member.count;
				break;
			case LayoutType::SIGNED_INT:
				size += sizeof(int32_t) * member.count;
				break;
			case LayoutType::FLOAT:
				size += sizeof(float) * member.count;
				break;
			case LayoutType::DOUBLE:
				size += sizeof(double) * member.count;
				break;
			case LayoutType::VECTOR2F:
				size += Vector2f::size() * member.count;
				break;
			case LayoutType::VECTOR3F:
				size += Vector3f::size() * member.count;
				break;
			case LayoutType::VECTOR4F:
				size += Vector4f::size() * member.count;
				break;
			}
		}

		return size;
	}
};

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer() {}

	virtual ~ShaderStorageBuffer() = 0;

	virtual void Store(void* data, const Layout& layout) = 0;

	virtual void Bind(void* commandBuffer, uint32_t binding) const = 0;
	virtual void Unbind() const = 0;

	virtual void MapBuffer() = 0;
	virtual void UnmapBuffer() = 0;

	void* getMappedData() const { return data; }

	virtual bool operator==(const ShaderStorageBuffer& other) = 0;
protected:
	void* data;
	Layout layout;
};

#endif