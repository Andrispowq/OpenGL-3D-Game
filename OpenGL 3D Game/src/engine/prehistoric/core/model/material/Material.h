#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>
#include "engine/prehistoric/core/math/Math.h"
#include "engine/prehistoric/core/util/TextureLoader.h"

class Material
{
public:
	Material();
	virtual ~Material();

	void AddTexture(const std::string& key, Texture* tex);

	Texture* GetTexture(const std::string& key) const;

	Vector4f* GetVector4f(const std::string& key) const;
	Vector3f* GetVector3f(const std::string& key) const;
	Vector2f* GetVector2f(const std::string& key) const;

	float* GetFloat(const std::string& key) const;
	int* GetInt(const std::string& key) const;

	static Texture* GetDef() { return DEF_TEXTURE; }
private:
	static Texture* DEF_TEXTURE;

	std::unordered_map<std::string, Texture*> textures;

	std::unordered_map<std::string, Vector4f*> vector4s;
	std::unordered_map<std::string, Vector3f*> vector3s;
	std::unordered_map<std::string, Vector2f*> vector2s;

	std::unordered_map<std::string, float*> floats;
	std::unordered_map<std::string, int*> ints;
};

#endif