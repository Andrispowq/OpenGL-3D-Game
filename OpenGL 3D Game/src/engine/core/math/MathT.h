template<typename T>
inline Vector2<T> Vector2<T>::abs() const
{
	return Vector2(fabs((*this).x), fabs((*this).y));
}

template<typename T>
Vector2<T> Vector2<T>::lerp(const Vector2<T>& b, const T& t, const bool& invert) const
{
	float x_ = x + (b.x - x) * t;
	float y_ = y + (b.y - y) * t;

	return invert ? Vector2<T>(y_, x_) : Vector2<T>(x_, y_);
}

template<typename T>
Vector2<T> Vector2<T>::lerp(const Vector2<T>& b, const Vector2<T>& t, const bool& invert) const
{
	float x_ = x + (b.x - x) * t.x;
	float y_ = y + (b.y - y) * t.y;

	return invert ? Vector2<T>(y_, x_) : Vector2<T>(x_, y_);
}

template<typename T>
Vector2<T> Vector2<T>::swap() const
{
	return Vector2<T>(y, x);
}

template<typename T>
inline Vector3<T> Vector3<T>::abs() const
{
	return Vector3<T>(std::abs(x), std::abs(y), std::abs(z));
}

template<typename T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& v) const
{
	float x_ = y * v.z - z * v.y;
	float y_ = z * v.x - x * v.z;
	float z_ = x * v.y - y * v.x;

	return Vector3<T>(x_, y_, z_);
}

template<typename T>
Vector3<T> Vector3<T>::reflect(const Vector3<T>& normal) const
{
	return *this - normal * 2 * this->dot(normal);
}

template<typename T>
Vector3<T> Vector3<T>::refract(const Vector3<T>& normal, const T& eta) const
{
	float k = 1 - std::pow(eta, 2) * (1 - std::pow(this->dot(normal), 2));

	if (k < 0)
		return Vector3<T>(0);
	else
		return *this * eta - normal * (eta * this->dot(normal) + std::sqrt(k));
}

template<typename T>
Vector3<T> Vector3<T>::lerp(const Vector3<T>& b, const T& t) const
{
	float x_ = x + (b.x - x) * t;
	float y_ = y + (b.y - y) * t;
	float z_ = z + (b.z - z) * t;

	return Vector3<T>(x_, y_, z_);
}

template<typename T>
Vector3<T> Vector3<T>::lerp(const Vector3<T>& b, const Vector3<T>& t) const
{
	float x_ = x + (b.x - x) * t.x;
	float y_ = y + (b.y - y) * t.y;
	float z_ = z + (b.z - z) * t.z;

	return Vector3<T>(x_, y_, z_);
}

template<typename T>
Vector3<T> Vector3<T>::rotate(const Vector3<T>& angles) const
{
	Vector3<T> rotX = this->rotate(Vector3<T>(1, 0, 0), angles.x);
	Vector3<T> rotY = this->rotate(Vector3<T>(0, 1, 0), angles.y);
	Vector3<T> rotZ = this->rotate(Vector3<T>(0, 0, 1), angles.z);

	return *this + rotX + rotY + rotZ;
}

template<typename T>
Vector3<T> Vector3<T>::rotate(const Vector3<T>& axis, const T& angle) const
{
	Vector3<T> result;

	if (axis.x != 0)
	{
		result = *this + Vector3<T>(0, this->x, x / std::tan(angle));
	}
	else if (axis.y != 0)
	{
		result = *this + Vector3<T>(this->x, 0, x / std::tan(angle));
	}
	else if (axis.z != 0)
	{
		result = *this + Vector3<T>(this->x, x / std::tan(angle), 0);
	}

	return *this + result;
}

template<typename T>
inline Vector4<T> Vector4<T>::abs() const
{
	return Vector4<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
}

template<typename T>
Vector4<T> Vector4<T>::lerp(const Vector4<T>& b, const T& t) const
{
	float x_ = x + (b.x - x) * t;
	float y_ = y + (b.y - y) * t;
	float z_ = z + (b.z - z) * t;
	float w_ = w + (b.w - w) * t;

	return Vector4<T>(x_, y_, z_, w_);
}

template<typename T>
Vector4<T> Vector4<T>::lerp(const Vector4<T>& b, const Vector4<T>& t) const
{
	float x_ = x + (b.x - x) * t.x;
	float y_ = y + (b.y - y) * t.y;
	float z_ = z + (b.z - z) * t.z;
	float w_ = w + (b.w - w) * t.w;

	return Vector4<T>(x_, y_, z_, w_);
}