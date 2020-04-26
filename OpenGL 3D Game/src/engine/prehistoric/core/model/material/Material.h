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

	virtual bool operator==(const Material& other)
	{
		if (textures.size() != other.textures.size() || vector4s.size() != other.vector4s.size() || vector3s.size() != other.vector3s.size()
			|| vector2s.size() != other.vector2s.size() || floats.size() != other.floats.size() || ints.size() != other.ints.size())
			return false;

		for (const auto& tex : textures)
		{
			if (other.textures.find(tex.first) == other.textures.end())
			{
				return false;
			}

			if (other.textures.at(tex.first) != tex.second)
			{
				return false;
			}
		}

		for (const auto& vec4 : vector4s)
		{
			if (other.vector4s.find(vec4.first) == other.vector4s.end())
			{
				return false;
			}

			if (other.vector4s.at(vec4.first) != vec4.second)
			{
				return false;
			}
		}
		for (const auto& vec3 : vector3s)
		{
			if (other.vector3s.find(vec3.first) == other.vector3s.end())
			{
				return false;
			}

			if (other.vector3s.at(vec3.first) != vec3.second)
			{
				return false;
			}
		}
		for (const auto& vec2 : vector2s)
		{
			if (other.vector2s.find(vec2.first) == other.vector2s.end())
			{
				return false;
			}

			if (other.vector2s.at(vec2.first) != vec2.second)
			{
				return false;
			}
		}

		for (const auto& fl : floats)
		{
			if (other.floats.find(fl.first) == other.floats.end())
			{
				return false;
			}

			if (other.floats.at(fl.first) != fl.second)
			{
				return false;
			}
		}
		for (const auto& in : ints)
		{
			if (other.ints.find(in.first) == other.ints.end())
			{
				return false;
			}

			if (other.ints.at(in.first) != in.second)
			{
				return false;
			}
		}

		return true;
	}

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