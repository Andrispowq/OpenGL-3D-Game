#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

class Material
{
public:
	Material(Window* window);
	virtual ~Material();

	static void CleanUp();

	void AddTexture(const std::string& key, Texture* value);
	void AddVector4f(const std::string& key, Vector4f* value);
	void AddVector3f(const std::string& key, Vector3f* value);
	void AddVector2f(const std::string& key, Vector2f* value);
	void AddFloat(const std::string& key, float* value);
	void AddInt(const std::string& key, int* value);

	Texture* GetTexture(const std::string& key) const;

	Vector4f GetVector4f(const std::string& key) const;
	Vector3f GetVector3f(const std::string& key) const;
	Vector2f GetVector2f(const std::string& key) const;

	float GetFloat(const std::string& key) const;
	int GetInt(const std::string& key) const;

	static Texture* GetDefaultTexture() { return listOfTextures[0]; }
	static Vector4f* GetDefaultVector4f() { return listOfVector4s[0]; }
	static Vector3f* GetDefaultVector3f() { return listOfVector3s[0]; }
	static Vector2f* GetDefaultVector2f() { return listOfVector2s[0]; }
	static float* GetDefaultFloat() { return listOfFloats[0]; }
	static int* GetDefaultInt() { return listOfInts[0]; }
private:
	static std::vector<Texture*> listOfTextures;
	static std::vector<Vector4f*> listOfVector4s;
	static std::vector<Vector3f*> listOfVector3s;
	static std::vector<Vector2f*> listOfVector2s;
	static std::vector<float*> listOfFloats;
	static std::vector<int*> listOfInts;

	std::unordered_map<std::string, size_t> textures;
	std::unordered_map<std::string, size_t> vector4s;
	std::unordered_map<std::string, size_t> vector3s;
	std::unordered_map<std::string, size_t> vector2s;
	std::unordered_map<std::string, size_t> floats;
	std::unordered_map<std::string, size_t> ints;
};

#endif