#ifndef MATH_H
#define MATH_H

#include "engine/prehistoric/core/util/Includes.hpp"
#include <xmmintrin.h>

#define PI 3.14159265358979323846264338327950288419716939

template<typename T>
static T ToDegrees(T x)
{
	return static_cast<T>((x / PI) * 180.0);
}

template<typename T>
static T ToRadians(T x)
{
	return static_cast<T>((x * PI) / 180.0);
}

template<typename T>
class Vector2
{
public:
	Vector2(const Vector2<T>& v) { this->x = v.x; this->y = v.y; }

	Vector2(const T& x, const T& y) : x(x), y(y) {}
	Vector2(const T& v) : x(v), y(v) {}
	Vector2() : x(0), y(0) {}

	~Vector2() {}

	inline Vector2<T> operator+(const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
	inline Vector2<T> operator-(const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
	inline Vector2<T> operator*(const Vector2<T>& v) const { return Vector2<T>(x * v.x, y * v.y); }
	inline Vector2<T> operator/(const Vector2<T>& v) const { return Vector2<T>(x / v.x, y / v.y); }

	inline Vector2<T> operator+(const T& v) const { return Vector2<T>(x + v, y + v); }
	inline Vector2<T> operator-(const T& v) const { return Vector2<T>(x - v, y - v); }
	inline Vector2<T> operator*(const T& v) const { return Vector2<T>(x * v, y * v); }
	inline Vector2<T> operator/(const T& v) const { return Vector2<T>(x / v, y / v); }

	inline Vector2<T> operator+=(const Vector2<T>& v) { return Vector2<T>(x += v.x, y += v.y); }
	inline Vector2<T> operator-=(const Vector2<T>& v) { return Vector2<T>(x -= v.x, y -= v.y); }
	inline Vector2<T> operator*=(const Vector2<T>& v) { return Vector2<T>(x *= v.x, y *= v.y); }
	inline Vector2<T> operator/=(const Vector2<T>& v) { return Vector2<T>(x /= v.x, y /= v.y); }

	inline Vector2<T> operator+=(const T& v) { return Vector2<T>(x += v, y += v); }
	inline Vector2<T> operator-=(const T& v) { return Vector2<T>(x -= v, y -= v); }
	inline Vector2<T> operator*=(const T& v) { return Vector2<T>(x *= v, y *= v); }
	inline Vector2<T> operator/=(const T& v) { return Vector2<T>(x /= v, y /= v); }

	inline T operator[](const int& index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else
			return 0;
	}

	inline bool operator<(const Vector2<T>& v) const { return x < v.x && y < v.x; }
	inline bool operator>(const Vector2<T>& v) const { return x > v.x && y > v.x; }
	inline bool operator<=(const Vector2<T>& v) const { return x <= v.x && y <= v.x; }
	inline bool operator>=(const Vector2<T>& v) const { return x >= v.x && y >= v.x; }
	inline bool operator==(const Vector2<T>& v) const { return x == v.x && y == v.y; }
	inline bool operator!=(const Vector2<T>& v) const { return !(operator==(v)); }

	inline T length() const { return sqrt(this->dot(*this)); }
	inline T lengthSquared() const { return this->dot(*this); }
	inline Vector2<T> normalize() const { return *this / length(); }
	inline T dot(const Vector2<T>& v) const { return x * v.x + y * v.y;	}

	inline Vector2<T> abs() const;

	Vector2<T> lerp(const Vector2<T>& b, const T& t, const bool& invert) const;
	Vector2<T> lerp(const Vector2<T>& b, const Vector2<T>& t, const bool& invert) const;

	Vector2<T> swap() const;

	Vector2<T> negate() const { return Vector2<T>(-x, -y); };
	Vector2<T> negated() { this->x = -x; this->y = -y; };

	void print() const { PR_LOG_MESSAGE("[ %f, %f ]\n", (float) x, (float) y); }
public:
	T x;
	T y;
};

template<typename T>
class Vector3
{
public:
	Vector3(const Vector3<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; }

	Vector3(const Vector2<T>& xy, const T& z) : x(xy.x), y(xy.y), z(z) {}
	Vector3(const T& x, const Vector2<T>& yz) : x(x), y(yz.x), z(yz.y) {}
	Vector3(const T& x, T y, T z) : x(x), y(y), z(z) {}
	Vector3(const T& v) : x(v), y(v), z(v) {}
	Vector3() : x(0), y(0), z(0) {}

	~Vector3() {}

	inline Vector3<T> operator+(const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
	inline Vector3<T> operator-(const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
	inline Vector3<T> operator*(const Vector3<T>& v) const { return Vector3<T>(x * v.x, y * v.y, z * v.z); }
	inline Vector3<T> operator/(const Vector3<T>& v) const { return Vector3<T>(x / v.x, y / v.y, z / v.z); }

	inline Vector3<T> operator+(const T& v) const { return Vector3<T>(x + v, y + v, z + v); }
	inline Vector3<T> operator-(const T& v) const { return Vector3<T>(x - v, y - v, z - v); }
	inline Vector3<T> operator*(const T& v) const { return Vector3<T>(x * v, y * v, z * v); }
	inline Vector3<T> operator/(const T& v) const { return Vector3<T>(x / v, y / v, z / v); }

	inline Vector3<T> operator+=(const Vector3<T>& v) { return Vector3<T>(x += v.x, y += v.y, z += v.z); }
	inline Vector3<T> operator-=(const Vector3<T>& v) { return Vector3<T>(x -= v.x, y -= v.y, z -= v.z); }
	inline Vector3<T> operator*=(const Vector3<T>& v) { return Vector3<T>(x *= v.x, y *= v.y, z *= v.z); }
	inline Vector3<T> operator/=(const Vector3<T>& v) { return Vector3<T>(x /= v.x, y /= v.y, z /= v.z); }
	
	inline Vector3<T> operator+=(const T& v) { return Vector3<T>(x += v, y += v, z += v); }
	inline Vector3<T> operator-=(const T& v) { return Vector3<T>(x -= v, y -= v, z -= v); }
	inline Vector3<T> operator*=(const T& v) { return Vector3<T>(x *= v, y *= v, z *= v); }
	inline Vector3<T> operator/=(const T& v) { return Vector3<T>(x /= v, y /= v, z /= v); }
	
	inline T operator[](const int& index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else if (index == 2)
			return z;
		else
			return 0;
	}

	inline bool operator<(const Vector3<T>& v) const { return x < v.x && y < v.x && z < v.z; }
	inline bool operator>(const Vector3<T>& v) const { return x > v.x && y > v.x && z > v.z; }
	inline bool operator<=(const Vector3<T>& v) const { return x <= v.x && y <= v.x && z <= v.z; }
	inline bool operator>=(const Vector3<T>& v) const { return x >= v.x && y >= v.x && z >= v.z; }
	inline bool operator==(const Vector3<T>& v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator!=(const Vector3<T>& v) const { return !(operator==(v)); }

	inline T length() const { return sqrt(this->dot(*this)); }
	inline T lengthSquared() const { return this->dot(*this); }
	inline Vector3<T> normalize() const { return *this / length(); }
	inline T dot(const Vector3<T>& v) const { return x * v.x + y * v.y + z * v.z; }

	inline Vector3<T> abs() const;

	Vector3<T> cross(const Vector3<T>& normal) const;
	Vector3<T> reflect(const Vector3<T>& normal) const;
	Vector3<T> refract(const Vector3<T>& normal, const T& eta) const;

	Vector3<T> lerp(const Vector3<T>& b, const T& t) const;
	Vector3<T> lerp(const Vector3<T>& b, const Vector3<T>& t) const;

	Vector3<T> rotate(const Vector3<T>& angles) const;
	Vector3<T> rotate(const Vector3<T>& axis, const T& angle) const;

	Vector3<T> negate() const { return Vector3<T>(-x, -y, -z); };
	Vector3<T> negated() { this->x = -x; this->y = -y; this->z = -z; };

	void print() const { PR_LOG_MESSAGE("[ %f, %f, %f ]\n", (float) x, (float) y, (float) z); }
public:
	T x;
	T y;
	T z;
};

template<typename T> class Quaternion;

template<typename T>
class Vector4
{
public:
	Vector4(const Vector4<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }
	Vector4(const Quaternion<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }

	Vector4(const T* v)
	{
		if (&(v[3]) == nullptr)
			PR_LOG_ERROR("You try to create a Vector4f object from an array of float with less then 4 members. The array's memory address: %i\n", v);

		this->x = v[0];
		this->y = v[1];
		this->z = v[2];
		this->w = v[3];
	}

	Vector4(const Vector3<T>& xyz, const T& w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	Vector4(const T& x, const Vector3<T>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
	Vector4(const Vector2<T>& xy, const T& z, const T& w) : x(xy.x), y(xy.y), z(w), w(w) {}
	Vector4(const Vector2<T>& xy, const Vector2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	Vector4(const T& x, const Vector2<T>& yz, const T& w) : x(x), y(yz.x), z(yz.y), w(w) {}
	Vector4(const T& x, const T& y, const Vector2<T>& zw) : x(x), y(y), z(zw.x), w(zw.y) {}
	Vector4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
	Vector4(const T& v) : x(v), y(v), z(v), w(v) {}
	Vector4() : x(0), y(0), z(0), w(0) {}

	~Vector4() {}

	inline Vector4<T> operator+(const Vector4<T>& v) const { return Vector4<T>(x + v.x, y + v.y, z + v.z, w + w); }
	inline Vector4<T> operator-(const Vector4<T>& v) const { return Vector4<T>(x - v.x, y - v.y, z - v.z, w - w); }
	inline Vector4<T> operator*(const Vector4<T>& v) const { return Vector4<T>(x * v.x, y * v.y, z * v.z, w * w); }
	inline Vector4<T> operator/(const Vector4<T>& v) const { return Vector4<T>(x / v.x, y / v.y, z / v.z, w / w); }

	inline Vector4<T> operator+(const T& v) const { return Vector4<T>(x + v, y + v, z + v, w + v); }
	inline Vector4<T> operator-(const T& v) const { return Vector4<T>(x - v, y - v, z - v, w - v); }
	inline Vector4<T> operator*(const T& v) const { return Vector4<T>(x * v, y * v, z * v, w * v); }
	inline Vector4<T> operator/(const T& v) const { return Vector4<T>(x / v, y / v, z / v, w / v); }

	inline Vector4<T> operator+=(const Vector4<T>& v) { return Vector4<T>(x += v.x, y += v.y, z += v.z, w += v.w); }
	inline Vector4<T> operator-=(const Vector4<T>& v) { return Vector4<T>(x -= v.x, y -= v.y, z -= v.z, w -= v.w); }
	inline Vector4<T> operator*=(const Vector4<T>& v) { return Vector4<T>(x *= v.x, y *= v.y, z *= v.z, w *= v.w); }
	inline Vector4<T> operator/=(const Vector4<T>& v) { return Vector4<T>(x /= v.x, y /= v.y, z /= v.z, w /= v.w); }

	inline Vector4<T> operator+=(const T& v) { return Vector4<T>(x += v, y += v, z += v, w += v); }
	inline Vector4<T> operator-=(const T& v) { return Vector4<T>(x -= v, y -= v, z -= v, w -= v); }
	inline Vector4<T> operator*=(const T& v) { return Vector4<T>(x *= v, y *= v, z *= v, w *= v); }
	inline Vector4<T> operator/=(const T& v) { return Vector4<T>(x /= v, y /= v, z /= v, w /= v); }

	inline T operator[](const int& index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else if (index == 2)
			return z;
		else if (index == 3)
			return w;
		else
			return 0;
	}

	inline bool operator<(const Vector4<T>& v) const { return x < v.x && y < v.x && z < v.z && w < v.w; }
	inline bool operator>(const Vector4<T>& v) const { return x > v.x && y > v.x && z > v.z && w > v.w; }
	inline bool operator<=(const Vector4<T>& v) const { return x <= v.x && y <= v.x && z <= v.z && w <= v.w; }
	inline bool operator>=(const Vector4<T>& v) const { return x >= v.x && y >= v.x && z >= v.z && w >= v.w; }
	inline bool operator==(const Vector4<T>& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	inline bool operator!=(const Vector4<T>& v) const { return !(operator==(v)); }

	inline T length() const { return sqrt(this->dot(*this)); }
	inline T lengthSquared() const { return this->dot(*this); }
	inline Vector4<T> normalize() const { return *this / length(); }
	inline T dot(const Vector4<T>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

	inline Vector4<T> abs() const;

	Vector4<T> lerp(const Vector4<T>& b, const T& t) const;
	Vector4<T> lerp(const Vector4<T>& b, const Vector4<T>& t) const;

	Vector4<T> negate() const { return Vector4<T>(-x, -y, -z, -w); };
	Vector4<T> negated() { this->x = -x; this->y = -y; this->z = -z; this->w = -w; };

	void print() const { PR_LOG_MESSAGE("[ %f, %f, %f, %f ]", (float) x, (float) y, (float) z, (float) w); }
public:
	T x;
	T y;
	T z;
	T w;
};

template<typename T>
class Quaternion
{
public:
	Quaternion(const Quaternion<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }
	Quaternion(const Vector4<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }

	Quaternion(const T* v)
	{
		if (&(v[3]) == nullptr)
			PR_LOG_ERROR("You try to create a Quaternion object from an array of float with less then 4 members. The array's memory address: %i\n", v);

		this->x = v[0];
		this->y = v[1];
		this->z = v[2];
		this->w = v[3];
	}

	Quaternion(const Vector3<T>& xyz, const T& w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	Quaternion(const T& x, const Vector3<T>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
	Quaternion(const Vector2<T>& xy, const T& z, const T& w) : x(xy.x), y(xy.y), z(w), w(w) {}
	Quaternion(const Vector2<T>& xy, const Vector2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	Quaternion(const T& x, const Vector2<T>& yz, const T& w) : x(x), y(yz.x), z(yz.y), w(w) {}
	Quaternion(const T& x, const T& y, const Vector2<T>& zw) : x(x), y(y), z(zw.x), w(zw.y) {}
	Quaternion(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
	Quaternion(const T& v) : x(v), y(v), z(v), w(v) {}
	Quaternion() : x(0), y(0), z(0), w(0) {}

	~Quaternion() {}

	inline Quaternion<T> operator+(const Vector4<T>& v) const { return Quaternion<T>(x + v.x, y + v.y, z + v.z, w + w); }
	inline Quaternion<T> operator-(const Vector4<T>& v) const { return Quaternion<T>(x - v.x, y - v.y, z - v.z, w - w); }
	inline Quaternion<T> operator+(const Quaternion<T>& v) const { return Quaternion<T>(x + v.x, y + v.y, z + v.z, w + w); }
	inline Quaternion<T> operator-(const Quaternion<T>& v) const { return Quaternion<T>(x - v.x, y - v.y, z - v.z, w - w); }

	inline Quaternion<T> operator+(const T& v) const { return Quaternion<T>(x + v, y + v, z + v, w + v); }
	inline Quaternion<T> operator-(const T& v) const { return Quaternion<T>(x - v, y - v, z - v, w - v); }
	inline Quaternion<T> operator*(const T& v) const { return Quaternion<T>(x * v, y * v, z * v, w * v); }
	inline Quaternion<T> operator/(const T& v) const { return Quaternion<T>(x / v, y / v, z / v, w / v); }

	inline Quaternion<T> operator+=(const Vector4<T>& v) { return Quaternion<T>(x += v.x, y += v.y, z += v.z, w += v.w); }
	inline Quaternion<T> operator-=(const Vector4<T>& v) { return Quaternion<T>(x -= v.x, y -= v.y, z -= v.z, w -= v.w); }
	inline Quaternion<T> operator+=(const Quaternion<T>& v) { return Quaternion<T>(x += v.x, y += v.y, z += v.z, w += v.w); }
	inline Quaternion<T> operator-=(const Quaternion<T>& v) { return Quaternion<T>(x -= v.x, y -= v.y, z -= v.z, w -= v.w); }

	inline Quaternion<T> operator+=(const T& v) { return Quaternion<T>(x += v, y += v, z += v, w += v); }
	inline Quaternion<T> operator-=(const T& v) { return Quaternion<T>(x -= v, y -= v, z -= v, w -= v); }
	inline Quaternion<T> operator*=(const T& v) { return Quaternion<T>(x *= v, y *= v, z *= v, w *= v); }
	inline Quaternion<T> operator/=(const T& v) { return Quaternion<T>(x /= v, y /= v, z /= v, w /= v); }

	inline Quaternion<T> Conjugate() const { return Quaternion(-x, -y, -z, w); }

	Quaternion<T> operator*(const Quaternion<T>& v) const;
	Quaternion<T> operator*(const Vector4<T>& v) const;
	Quaternion<T> operator*(const Vector3<T>& v) const;

	Quaternion<T> operator*=(const Quaternion<T>& v) const;
	Quaternion<T> operator*=(const Vector4<T>& v) const;
	Quaternion<T> operator*=(const Vector3<T>& v) const;

	inline T operator[](const int& index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else if (index == 2)
			return z;
		else if (index == 3)
			return w;
		else
			return 0;
	}

	inline bool operator<(const Quaternion<T>& v) const { return x < v.x&& y < v.x&& z < v.z&& w < v.w; }
	inline bool operator>(const Quaternion<T>& v) const { return x > v.x && y > v.x && z > v.z && w > v.w; }
	inline bool operator<=(const Quaternion<T>& v) const { return x <= v.x && y <= v.x && z <= v.z && w <= v.w; }
	inline bool operator>=(const Quaternion<T>& v) const { return x >= v.x && y >= v.x && z >= v.z && w >= v.w; }
	inline bool operator==(const Quaternion<T>& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	inline bool operator!=(const Quaternion<T>& v) const { return !(operator==(v)); }

	inline T length() const { return sqrt(this->dot(*this)); }
	inline T lengthSquared() const { return this->dot(*this); }
	inline Quaternion<T> normalize() const { return *this / length(); }
	inline T dot(const Quaternion<T>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

	inline Quaternion<T> abs() const;

	Quaternion<T> lerp(const Quaternion<T>& b, const T& t) const;
	Quaternion<T> lerp(const Quaternion<T>& b, const Quaternion<T>& t) const;

	Quaternion<T> negate() const { return Quaternion<T>(-x, -y, -z, -w); };
	Quaternion<T> negated() { this->x = -x; this->y = -y; this->z = -z; this->w = -w; };

	void print() const { PR_LOG_MESSAGE("[ %f, %f, %f, %f ]", (float) x, (float) y, (float) z, (float) w); }
public:
	T x;
	T y;
	T z;
	T w;
};

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;
typedef Quaternion<int> Quaternioni;

typedef Vector2<uint32_t> Vector2u;
typedef Vector3<uint32_t> Vector3u;
typedef Vector4<uint32_t> Vector4u;
typedef Quaternion<uint32_t> Quaternionu;

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;
typedef Quaternion<float> Quaternionf;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;
typedef Quaternion<double> Quaterniond;

class Matrix4f
{
public:
	Matrix4f(const Matrix4f& v);

	Matrix4f(const Vector4f& v);
	Matrix4f();

	~Matrix4f();

	Matrix4f operator=(const Matrix4f& v);

	inline Matrix4f operator+(const Matrix4f& v) const;
	inline Matrix4f operator-(const Matrix4f& v) const;

	inline Matrix4f operator+=(const Matrix4f& v);
	inline Matrix4f operator-=(const Matrix4f& v);

	inline Matrix4f operator+(const Vector4f& v) const;
	inline Matrix4f operator-(const Vector4f& v) const;

	inline Matrix4f operator+=(const Vector4f& v);
	inline Matrix4f operator-=(const Vector4f& v);

	inline Matrix4f operator+(const float& v) const;
	inline Matrix4f operator-(const float& v) const;

	inline Matrix4f operator+=(const float& v);
	inline Matrix4f operator-=(const float& v);

	inline Matrix4f operator*(const Matrix4f& v) const;
	inline Matrix4f operator*=(const Matrix4f& v);

	Vector4f operator*(const Vector4f& v) const;

	inline bool operator==(const Matrix4f& v) const;

	float* operator[](const int& index) const;

	static Matrix4f Identity();
	static Matrix4f Zero();

	static Matrix4f Translation(const Vector3f& position);
	static Matrix4f Rotation(const Vector3f& rotation);
	static Matrix4f Scaling(const Vector3f& scale);

	static Matrix4f Transformation(const Vector3f& translation, const Vector3f& rotation, const Vector3f& scaling);
	static Matrix4f PerspectiveProjection(const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane);
	static Matrix4f View(const Vector3f& forward, const Vector3f& up);

	float* const getRow(const int i) const;

	float* GetData() const;

	void print() const
	{
		PR_LOG_MESSAGE("Matrix: \n");
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				PR_LOG_MESSAGE("\t%f", m[j * 4 + i]);
			}

			PR_LOG_MESSAGE("\n");
		}
	}
private:
	void clear();
public:
	float* m;
};

#endif 