#include "Material.h"
#include "engine/prehistoric/common/model/Texture.h"

Texture* Material::DEF_TEXTURE;

Material::Material()
{
#ifndef DEF_INIT
#define DEF_INIT
	DEF_TEXTURE = TextureLoader::LoadTexture("res/textures/default.png");
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

void Material::AddTexture(const std::string& key, Texture* texture)
{
	textures.emplace(key, texture);
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

Vector4f* Material::GetVector4f(const std::string& key) const
{
	Vector4f* out = GetElement(vector4s, key);

	if (out == nullptr)
		out = &(Vector4f());

	return out;
}

Vector3f* Material::GetVector3f(const std::string& key) const
{
	Vector3f* out = GetElement(vector3s, key);

	if (out == nullptr)
		out = &(Vector3f());

	return out;
}

Vector2f* Material::GetVector2f(const std::string& key) const
{
	Vector2f* out = GetElement(vector2s, key);

	if (out == nullptr)
		out = &(Vector2f());

	return out;
}

float* Material::GetFloat(const std::string& key) const
{
	float* out = GetElement(floats, key);

	if (out == nullptr)
	{
		float f = -1;
		out = &f;
	}

	return out;
}

int* Material::GetInt(const std::string& key) const
{
	int* out = GetElement(ints, key);

	if (out == nullptr)
	{
		int f = -1;
		out = &f;
	}

	return out;
}