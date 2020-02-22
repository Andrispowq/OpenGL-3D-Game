#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <iostream>

#define PI 3.141592653589793

#define ToRadians(x) (x * PI) / 180
#define ToDegrees(x) (x * 180) / PI

template<typename T>
class Vector2
{
public:
	Vector2(const Vector2<T>& v) { this->x = v.x; this->y = v.y; }

	Vector2(const T& x, const T& y) : x(x), y(y) {}
	Vector2(const T& v) : x(v), y(v) {}
	Vector2() : x(0), y(0) {}

	virtual ~Vector2() {}

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

	inline T& operator[](const int index) const
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

	inline T length() const;
	inline T lengthSquared() const;
	inline Vector2<T> normalize() const;

	inline T dot(const Vector2<T>& v) const;

	inline Vector2<T> abs() const;

	Vector2<T> lerp(const Vector2<T>& b, const T& t, const bool& invert) const;
	Vector2<T> lerp(const Vector2<T>& b, const Vector2<T>& t, const bool& invert) const;

	Vector2<T> swap() const;

	Vector2<T> negate() const { return Vector2<T>(-x, -y); };
	Vector2<T> negated() { this->x = -x; this->y = -y; };

	void print() const { std::cout << "X: " << x << ", Y: " << y << std::endl; }
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

	virtual ~Vector3() {}

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
	
	inline T& operator[](const int index) const
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
	
	inline T length() const;
	inline T lengthSquared() const;
	inline Vector3<T> normalize() const;
	inline T dot(Vector3<T> v) const;

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

	void print() const { std::cout << "X: " << x << ", Y: " << y << ", Z: " << z << std::endl; }
public:
	T x;
	T y;
	T z;
};

template<typename T>
class Vector4
{
public:
	Vector4(const Vector4<T>& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }

	Vector4(const T* v)
	{
		if (&(v[3]) == nullptr)
			std::cerr << "You try to create a Vector4f object from an array of float with less then 4 members. The array's memory address: " << v << std::endl;

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

	virtual ~Vector4() {}

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

	inline T& operator[](const int index) const
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

	inline T length() const;
	inline T lengthSquared() const;
	inline Vector4<T> normalize() const;

	inline T dot(const Vector4<T>& v) const;

	inline Vector4<T> abs() const;

	Vector4<T> lerp(const Vector4<T>& b, const T& t) const;
	Vector4<T> lerp(const Vector4<T>& b, const Vector4<T>& t) const;

	Vector4<T> negate() const { return Vector4<T>(-x, -y, -z, -w); };
	Vector4<T> negated() { this->x = -x; this->y = -y; this->z = -z; this->w = -w; };

	void print() const { std::cout << "X: " << x << ", Y: " << y << ", Z: " << z << ", W: " << w << std::endl; }
public:
	T x;
	T y;
	T z;
	T w;
};

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

typedef Vector2<unsigned int> Vector2ui;
typedef Vector3<unsigned int> Vector3ui;
typedef Vector4<unsigned int> Vector4ui;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

class Matrix4f
{
public:
	Matrix4f(const Matrix4f& m);

	Matrix4f(Vector4f v);
	Matrix4f();

	virtual ~Matrix4f();

	inline Matrix4f operator+(Matrix4f v) const;
	inline Matrix4f operator-(Matrix4f v) const;

	inline Matrix4f operator+=(Matrix4f v);
	inline Matrix4f operator-=(Matrix4f v);

	inline Matrix4f operator+(Vector4f v) const;
	inline Matrix4f operator-(Vector4f v) const;

	inline Matrix4f operator+=(Vector4f v);
	inline Matrix4f operator-=(Vector4f v);

	inline Matrix4f operator+(float v) const;
	inline Matrix4f operator-(float v) const;

	inline Matrix4f operator+=(float v);
	inline Matrix4f operator-=(float v);

	inline Matrix4f operator*(Matrix4f v) const;
	inline Matrix4f operator*=(Matrix4f v);

	inline Vector4f operator*(Vector4f v) const;

	inline float * operator[](const int index) const
	{
		return m[index];
	}

	inline Matrix4f Identity() const;
	inline Matrix4f Zero() const;

	Matrix4f Translation(Vector3f position) const;
	Matrix4f Rotation(Vector3f rotation) const;
	Matrix4f Scaling(Vector3f scale) const;

	Matrix4f Transformation(Vector3f translation, Vector3f rotation, Vector3f scaling) const;
	Matrix4f PerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane) const;

	inline float * const getRow(int i) const;
public:
	float **m;
};

#endif 