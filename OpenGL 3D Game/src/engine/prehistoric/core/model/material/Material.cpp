#include "engine/prehistoric/core/util/Includes.hpp"
#include "Material.h"
#include "engine/prehistoric/common/model/Texture.h"

Texture* Material::DEF_TEXTURE;

Vector4f* Material::DEF_VECTOR4F;
Vector3f* Material::DEF_VECTOR3F;
Vector2f* Material::DEF_VECTOR2F;

float* Material::DEF_FLOAT;
int* Material::DEF_INT;

Material::Material()
{
#ifndef DEFS_INIT
#define DEFS_INIT
	DEF_TEXTURE = TextureLoader::LoadTexture("res/textures/default.png");

	DEF_VECTOR4F = new Vector4f(-1);
	DEF_VECTOR3F = new Vector3f(-1);
	DEF_VECTOR2F = new Vector2f(-1);

	DEF_FLOAT = new float;
	(*DEF_FLOAT) = -1;

	DEF_INT = new int;
	(*DEF_INT) = -1;
#endif
}

Material::~Material()
{
	for (auto kv : textures)
	{
		delete kv.second;
	}

	for (auto kv : vector4s)
	{
		delete kv.second;
	}
	for (auto kv : vector3s)
	{
		delete kv.second;
	}
	for (auto kv : vector2s)
	{
		delete kv.second;
	}

	for (auto kv : floats)
	{
		delete kv.second;
	}
	for (auto kv : ints)
	{
		delete kv.second;
	}
}

void Material::CleanUp()
{
	delete DEF_TEXTURE;

	delete DEF_VECTOR4F;
	delete DEF_VECTOR3F;
	delete DEF_VECTOR2F;

	delete DEF_FLOAT;
	delete DEF_INT;
}

void Material::AddTexture(const std::string& key, Texture* value)
{
	textures.emplace(key, value);
}

void Material::AddVector4f(const std::string& key, Vector4f* value)
{
	vector4s.emplace(key, value);
}

void Material::AddVector3f(const std::string& key, Vector3f* value)
{
	vector3s.emplace(key, value);
}

void Material::AddVector2f(const std::string& key, Vector2f* value)
{
	vector2s.emplace(key, value);
}

void Material::AddFloat(const std::string& key, float* value)
{
	floats.emplace(key, value);
}

void Material::AddInt(const std::string& key, int* value)
{
	ints.emplace(key, value);
}

template<typename T>
static T* GetElement(std::unordered_map<std::string, T*> map, const std::string key)
{
	auto index = map.find(key);

	if (index == map.end())
		return nullptr;
	else
		return map.at(key);
}

Texture* Material::GetTexture(const std::string& key) const
{
	Texture* out = GetElement(textures, key);

	if (out == nullptr)
		out = DEF_TEXTURE;

	return out;
}

Vector4f Material::GetVector4f(const std::string& key) const
{
	Vector4f* out = GetElement(vector4s, key);

	if (out == nullptr)
		out = new Vector4f(-1);

	return *out;
}

Vector3f Material::GetVector3f(const std::string& key) const
{
	Vector3f* out = GetElement(vector3s, key);

	if (out == nullptr)
		out = new Vector3f(-1);

	return *out;
}

Vector2f Material::GetVector2f(const std::string& key) const
{
	Vector2f* out = GetElement(vector2s, key);

	if (out == nullptr)
		out = new Vector2f(-1);

	return *out;
}

float Material::GetFloat(const std::string& key) const
{
	float* out = GetElement(floats, key);

	if (out == nullptr)
	{
		float f = -1;
		out = &f;
	}

	return *out;
}

int Material::GetInt(const std::string& key) const
{
	int* out = GetElement(ints, key);

	if (out == nullptr)
	{
		int f = -1;
		out = &f;
	}

	return *out;
}