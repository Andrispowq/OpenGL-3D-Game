#include "engine/prehistoric/core/util/Includes.hpp"
#include "Material.h"

std::vector<Texture*> Material::listOfTextures;
std::vector<Vector4f*> Material::listOfVector4s;
std::vector<Vector3f*> Material::listOfVector3s;
std::vector<Vector2f*> Material::listOfVector2s;
std::vector<float*> Material::listOfFloats;
std::vector<int*> Material::listOfInts;

Material::Material(Window* window)
{
	if (listOfTextures.size() == 0)
	{
		listOfTextures.push_back(TextureLoader::LoadTexture("res/textures/default.png", window));
	}
	if (listOfVector4s.size() == 0)
	{
		listOfVector4s.push_back(new Vector4f(-1));
	}
	if (listOfVector3s.size() == 0)
	{
		listOfVector3s.push_back(new Vector3f(-1));
	}
	if (listOfVector2s.size() == 0)
	{
		listOfVector2s.push_back(new Vector2f(-1));
	}
	if (listOfFloats.size() == 0)
	{
		listOfFloats.push_back(new float(-1));
	}
	if (listOfInts.size() == 0)
	{
		listOfInts.push_back(new int(-1));
	}
}

Material::~Material()
{
}

void Material::CleanUp()
{
	for (auto& tex : listOfTextures)
	{
		delete tex;
	}
	for (auto& vec4 : listOfVector4s)
	{
		delete vec4;
	}
	for (auto& vec3 : listOfVector3s)
	{
		delete vec3;
	}
	for (auto& vec2 : listOfVector2s)
	{
		delete vec2;
	}
	for (auto& flo : listOfFloats)
	{
		delete flo;
	}
	for (auto& in : listOfInts)
	{
		delete in;
	}
}

void Material::AddTexture(const std::string& key, Texture* value)
{
	auto index = std::find(listOfTextures.begin(), listOfTextures.end(), value);
	if (index == listOfTextures.end())
	{
		listOfTextures.push_back(value);
		textures.insert(std::make_pair(key, listOfTextures.size() - 1));
	}
	else
	{
		textures.insert(std::make_pair(key, std::distance(listOfTextures.begin(), index)));
	}
}

void Material::AddVector4f(const std::string& key, Vector4f* value)
{
	auto index = std::find(listOfVector4s.begin(), listOfVector4s.end(), value);
	if (index == listOfVector4s.end())
	{
		listOfVector4s.push_back(value);
		vector4s.insert(std::make_pair(key, listOfVector4s.size() - 1));
	}
	else
	{
		vector4s.insert(std::make_pair(key, std::distance(listOfVector4s.begin(), index)));
	}
}

void Material::AddVector3f(const std::string& key, Vector3f* value)
{
	auto index = std::find(listOfVector3s.begin(), listOfVector3s.end(), value);
	if (index == listOfVector3s.end())
	{
		listOfVector3s.push_back(value);
		vector3s.insert(std::make_pair(key, listOfVector3s.size() - 1));
	}
	else
	{
		vector3s.insert(std::make_pair(key, std::distance(listOfVector3s.begin(), index)));
	}
}

void Material::AddVector2f(const std::string& key, Vector2f* value)
{
	auto index = std::find(listOfVector2s.begin(), listOfVector2s.end(), value);
	if (index == listOfVector2s.end())
	{
		listOfVector2s.push_back(value);
		vector2s.insert(std::make_pair(key, listOfVector2s.size() - 1));
	}
	else
	{
		vector2s.insert(std::make_pair(key, std::distance(listOfVector2s.begin(), index)));
	}
}

void Material::AddFloat(const std::string& key, float* value)
{
	auto index = std::find(listOfFloats.begin(), listOfFloats.end(), value);
	if (index == listOfFloats.end())
	{
		listOfFloats.push_back(value);
		floats.insert(std::make_pair(key, listOfFloats.size() - 1));
	}
	else
	{
		floats.insert(std::make_pair(key, std::distance(listOfFloats.begin(), index)));
	}
}

void Material::AddInt(const std::string& key, int* value)
{
	auto index = std::find(listOfInts.begin(), listOfInts.end(), value);
	if (index == listOfInts.end())
	{
		listOfInts.push_back(value);
		ints.insert(std::make_pair(key, listOfInts.size() - 1));
	}
	else
	{
		ints.insert(std::make_pair(key, std::distance(listOfInts.begin(), index)));
	}
}

template<typename T>
static T* GetElement(std::unordered_map<std::string, size_t> map, std::vector<T*> list, const std::string key)
{
	auto index = map.find(key);

	if (index == map.end())
	{
		return nullptr;
	}
	else
	{
		size_t i = map[key];

		if (list.size() > i)
		{
			return list[i];
		}
		else
		{
			return nullptr;
		}
	}
}

Texture* Material::GetTexture(const std::string& key) const
{
	Texture* out = GetElement(textures, listOfTextures, key);

	if (out == nullptr)
		out = listOfTextures[0];

	return out;
}

Vector4f Material::GetVector4f(const std::string& key) const
{
	Vector4f* out = GetElement(vector4s, listOfVector4s, key);

	if (out == nullptr)
		out = listOfVector4s[0];

	return *out;
}

Vector3f Material::GetVector3f(const std::string& key) const
{
	Vector3f* out = GetElement(vector3s, listOfVector3s, key);

	if (out == nullptr)
		out = listOfVector3s[0];

	return *out;
}

Vector2f Material::GetVector2f(const std::string& key) const
{
	Vector2f* out = GetElement(vector2s, listOfVector2s, key);

	if (out == nullptr)
		out = listOfVector2s[0];

	return *out;
}

float Material::GetFloat(const std::string& key) const
{
	float* out = GetElement(floats, listOfFloats, key);

	if (out == nullptr)
		out = listOfFloats[0];

	return *out;
}

int Material::GetInt(const std::string& key) const
{
	int* out = GetElement(ints, listOfInts, key);

	if (out == nullptr)
		out = listOfInts[0];

	return *out;
}