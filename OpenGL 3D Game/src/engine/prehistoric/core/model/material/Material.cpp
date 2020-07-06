#include "engine/prehistoric/core/util/Includes.hpp"
#include "Material.h"

std::vector<Texture*> Material::listOfTextures;

Material::Material(Window* window)
{
	if (listOfTextures.size() == 0)
	{
		listOfTextures.push_back(TextureLoader::LoadTexture("res/textures/default.png", window));
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
}

void Material::AddTexture(const std::string& key, Texture* value)
{
	auto index = std::find(listOfTextures.begin(), listOfTextures.end(), value);
	if (index == listOfTextures.end())
	{
		listOfTextures.push_back(value);
	}

	textures.insert(std::make_pair(key, value));
}

void Material::AddVector4f(const std::string& key, Vector4f value)
{
	vector4s.insert(std::make_pair(key, value));
}

void Material::AddVector3f(const std::string& key, Vector3f value)
{
	vector3s.insert(std::make_pair(key, value));
}

void Material::AddVector2f(const std::string& key, Vector2f value)
{
	vector2s.insert(std::make_pair(key, value));
}

void Material::AddFloat(const std::string& key, float value)
{
	floats.insert(std::make_pair(key, value));
}

void Material::AddInt(const std::string& key, int value)
{
	ints.insert(std::make_pair(key, value));
}

Texture* Material::GetTexture(const std::string& key) const
{
	auto index = textures.find(key);
	if (index == textures.end())
		 return listOfTextures[0];
	
	return index->second;
}

Vector4f Material::GetVector4f(const std::string& key) const
{
	auto index = vector4s.find(key);
	if (index == vector4s.end())
		return Vector4f(-1);

	return index->second;
}

Vector3f Material::GetVector3f(const std::string& key) const
{
	auto index = vector3s.find(key);
	if (index == vector3s.end())
		return Vector3f(-1);

	return index->second;
}

Vector2f Material::GetVector2f(const std::string& key) const
{
	auto index = vector2s.find(key);
	if (index == vector2s.end())
		return Vector2f(-1);

	return index->second;
}

float Material::GetFloat(const std::string& key) const
{
	auto index = floats.find(key);
	if (index == floats.end())
		return -1.0;

	return index->second;
}

int Material::GetInt(const std::string& key) const
{
	auto index = ints.find(key);
	if (index == ints.end())
		return -1;

	return index->second;
}