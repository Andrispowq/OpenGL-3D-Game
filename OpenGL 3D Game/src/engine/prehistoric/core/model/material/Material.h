#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

#include "engine/prehistoric/assets/AssetManager.h"

class Material
{
public:
	Material(AssetManager* manager, Window* window);
	~Material() {}

	void AddTexture(const std::string& key, size_t textureID);
	void AddVector4f(const std::string& key, Vector4f value);
	void AddVector3f(const std::string& key, Vector3f value);
	void AddVector2f(const std::string& key, Vector2f value);
	void AddFloat(const std::string& key, float value);
	void AddInt(const std::string& key, int value);

	Texture* GetTexture(const std::string& key) const;

	Vector4f GetVector4f(const std::string& key) const;
	Vector3f GetVector3f(const std::string& key) const;
	Vector2f GetVector2f(const std::string& key) const;

	float GetFloat(const std::string& key) const;
	int GetInt(const std::string& key) const;
private:
	AssetManager* manager;

	std::unordered_map<std::string, size_t> textureIDs;
	std::unordered_map<std::string, Vector4f> vector4s;
	std::unordered_map<std::string, Vector3f> vector3s;
	std::unordered_map<std::string, Vector2f> vector2s;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, int> ints;
};

#endif