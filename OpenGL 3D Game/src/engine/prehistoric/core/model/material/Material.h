#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/util/loader/TextureLoader.h"

class Material
{
public:
	Material();
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

	static Texture* GetDefaultTexture() { return DEF_TEXTURE; }
	static Vector4f* GetDefaultVector4f() { return DEF_VECTOR4F; }
	static Vector3f* GetDefaultVector3f() { return DEF_VECTOR3F; }
	static Vector2f* GetDefaultVector2f() { return DEF_VECTOR2F; }
	static float* GetDefaultFloat() { return DEF_FLOAT; }
	static int* GetDefaultInt() { return DEF_INT; }
private:
	static Texture* DEF_TEXTURE;
	static Vector4f* DEF_VECTOR4F;
	static Vector3f* DEF_VECTOR3F;
	static Vector2f* DEF_VECTOR2F;
	static float* DEF_FLOAT;
	static int* DEF_INT;

	std::unordered_map<std::string, Texture*> textures;

	std::unordered_map<std::string, Vector4f*> vector4s;
	std::unordered_map<std::string, Vector3f*> vector3s;
	std::unordered_map<std::string, Vector2f*> vector2s;

	std::unordered_map<std::string, float*> floats;
	std::unordered_map<std::string, int*> ints;
};

#endif