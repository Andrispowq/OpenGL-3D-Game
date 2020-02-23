#include "Math.h"

Matrix4f::Matrix4f(const Matrix4f& m)
{
	this->m = new float*[4];

	for (int i = 0; i < 4; i++)
	{
		this->m[i] = new float[4];
		memset(this->m[i], 0, sizeof(float) * 4);
	}

	for (int i = 0; i < 4; i++)
	{
		memcpy(this->m[i], m.m[i], sizeof(float) * 4);
	}
}

Matrix4f::Matrix4f(Vector4f v)
{
	this->m = new float*[4];

	for (int i = 0; i < 4; i++)
	{
		this->m[i] = new float[4];
		memset(this->m[i], 0, sizeof(float) * 4);
	}

	for (int i = 0; i < 4; i++)
	{
		m[0][i] = v.x;
		m[1][i] = v.y;
		m[2][i] = v.z;
		m[3][i] = v.w;
	}
}

Matrix4f::Matrix4f()
{
	this->m = new float*[4];

	for (int i = 0; i < 4; i++)
	{
		this->m[i] = new float[4];
		memset(this->m[i], 0, sizeof(float) * 4);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] = 0;
		}
	}
}

Matrix4f::~Matrix4f()
{
	for (int i = 0; i < 4; i++)
	{
 		delete[] m[i];
	}

	delete[] m;
}

inline Matrix4f Matrix4f::operator+(Matrix4f v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = m[i][j] + v.m[i][j];
		}
	}

	return res;
}

inline Matrix4f Matrix4f::operator-(Matrix4f v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = m[i][j] - v.m[i][j];
		}
	}

	return res;
}

inline Matrix4f Matrix4f::operator+=(Matrix4f v)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += v.m[i][j];
		}
	}

	return *this;
}

inline Matrix4f Matrix4f::operator-=(Matrix4f v)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] -= v.m[i][j];
		}
	}

	return *this;
}

inline Matrix4f Matrix4f::operator+(Vector4f v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		res.m[i][0] = m[i][0] + v.x;
		res.m[i][1] = m[i][1] + v.y;
		res.m[i][2] = m[i][2] + v.z;
		res.m[i][3] = m[i][3] + v.w;
	}

	return res;
}

inline Matrix4f Matrix4f::operator-(Vector4f v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		res.m[i][0] = m[i][0] - v.x;
		res.m[i][1] = m[i][1] - v.y;
		res.m[i][2] = m[i][2] - v.z;
		res.m[i][3] = m[i][3] - v.w;
	}

	return res;
}

inline Matrix4f Matrix4f::operator+=(Vector4f v)
{
	for (int i = 0; i < 4; i++)
	{
		m[i][0] += v.x;
		m[i][1] += v.y;
		m[i][2] += v.z;
		m[i][3] += v.w;
	}

	return *this;
}

inline Matrix4f Matrix4f::operator-=(Vector4f v)
{
	for (int i = 0; i < 4; i++)
	{
		m[i][0] -= v.x;
		m[i][1] -= v.y;
		m[i][2] -= v.z;
		m[i][3] -= v.w;
	}

	return *this;
}

inline Matrix4f Matrix4f::operator+(float v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = m[i][j] + v;
		}
	}

	return res;
}

inline Matrix4f Matrix4f::operator-(float v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = m[i][j] - v;
		}
	}

	return res;
}

inline Matrix4f Matrix4f::operator+=(float v)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += v;
		}
	}

	return *this;
}

inline Matrix4f Matrix4f::operator-=(float v)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] -= v;
		}
	}

	return *this;
}

inline Matrix4f Matrix4f::operator*(Matrix4f v) const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//j -> this matrix
			//i -> v matrix
			res.m[i][j] = v[i][0] * m[0][j] + v[i][1] * m[1][j] + v[i][2] * m[2][j] + v[i][3] * m[3][j];
		}
	}

	return res;
}

inline Matrix4f Matrix4f::operator*=(Matrix4f v)
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = m[i][0] * v.m[0][j] + m[i][1] * v.m[1][j] + m[i][2] * v.m[2][j] + m[i][3] * v.m[3][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] = res.m[i][j];
		}
	}

	return *this;
}

Vector4f Matrix4f::operator*(Vector4f v) const
{
	Vector4f res = Vector4f();

	res.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
	res.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
	res.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
	res.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;

	return res;
}

Matrix4f Matrix4f::Identity() const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				res.m[i][j] = 1;
			else
				res.m[i][j] = 0;
		}
	}

	return res;
}

Matrix4f Matrix4f::Zero() const
{
	Matrix4f res = Matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.m[i][j] = 0;
		}
	}

	return res;
}

Matrix4f Matrix4f::Translation(Vector3f position) const
{
	Matrix4f res = Matrix4f();

	res.m[3][0] = position.x;
	res.m[3][1] = position.y;
	res.m[3][2] = position.z;

	return res;
}

Matrix4f Matrix4f::Rotation(Vector3f rotation) const
{
	Matrix4f rx = Matrix4f().Identity();
	Matrix4f ry = Matrix4f().Identity();
	Matrix4f rz = Matrix4f().Identity();

	float x = ToRadians(rotation.x);
	float y = ToRadians(rotation.y);
	float z = ToRadians(rotation.z);

	rx.m[1][1] = cos(x);
	rx.m[2][1] = -sin(x);
	rx.m[1][2] = sin(x);
	rx.m[2][2] = cos(x);

	ry.m[0][0] = cos(y);
	ry.m[2][0] = sin(y);
	ry.m[0][2] = -sin(y);
	ry.m[2][2] = cos(y);

	rz.m[0][0] = cos(z);
	rz.m[1][0] = -sin(z);
	rz.m[0][1] = sin(z);
	rz.m[1][1] = cos(z);

	return ry * rx * rz;
}

Matrix4f Matrix4f::Scaling(Vector3f scale) const
{
	Matrix4f res = Matrix4f().Identity();

	res.m[0][0] = scale.x;
	res.m[1][1] = scale.y;
	res.m[2][2] = scale.z;

	return res;
}

Matrix4f Matrix4f::Transformation(Vector3f translation, Vector3f rotation, Vector3f scaling) const
{
	Matrix4f Translation = Matrix4f().Translation(translation);
	Matrix4f Rotation = Matrix4f().Rotation(rotation);
	Matrix4f Scaling = Matrix4f().Scaling(scaling);

	return Scaling * Rotation * Translation;
}

Matrix4f Matrix4f::PerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane) const
{
	Matrix4f res = Matrix4f().Identity();

	float tanFOV = tan(ToRadians(fov / 2));
	float frustumLength = farPlane - nearPlane;

	res.m[0][0] = 1 / (tanFOV * aspectRatio);
	res.m[1][1] = 1 / tanFOV;
	res.m[2][2] = (farPlane + nearPlane) / frustumLength;
	res.m[3][2] = -(2 * farPlane * nearPlane) / frustumLength;
	res.m[2][3] = 1;
	res.m[3][3] = 0;

	return res;
}

Matrix4f Matrix4f::View(Vector3f forward, Vector3f up) const
{
	Vector3f right = up.cross(forward);

	Matrix4f mat = Matrix4f().Identity();

	mat[0][0] = right.x;
	mat[0][1] = right.y;
	mat[0][2] = right.z;

	mat[1][0] = up.x;
	mat[1][1] = up.y;
	mat[1][2] = up.z;

	mat[2][0] = forward.x;
	mat[2][1] = forward.y;
	mat[2][2] = forward.z;

	return mat;
}

/*
	The parameter <i> is the index of the row, so to get the first row, the parameter MUST be 0
*/
float * const Matrix4f::getRow(int i) const
{
	float* ptr = new float[4];

	for (int j = 0; j < 4; j++)
	{
		ptr[j] = m[j][i];
	}

	return ptr;
}