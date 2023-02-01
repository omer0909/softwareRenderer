#pragma once

#include <iostream>
#include <math.h>

class Vector3
{
      public:
	inline Vector3() {}

	inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	inline Vector3(const Vector3 &a) { *this = a; }

	inline Vector3 &operator=(const Vector3 &a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	inline float SqrMagnitude() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	inline float Magnitude() const { return std::sqrt(SqrMagnitude()); }

	inline Vector3 operator+(const Vector3 &a) const
	{
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	inline Vector3 operator-(const Vector3 &a) const
	{
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	inline Vector3 operator*(const Vector3 &a) const
	{
		return Vector3(x * a.x, y * a.y, z * a.z);
	}

	inline Vector3 operator/(const Vector3 &a) const
	{
		return Vector3(x / a.x, y / a.y, z / a.z);
	}

	inline Vector3 operator*(const float &a) const
	{
		return Vector3(x * a, y * a, z * a);
	}

	inline Vector3 operator/(const float &a) const
	{
		const float factor = 1.0f / a;
		return Vector3(x * factor, y * factor, z * factor);
	}

	inline Vector3 operator-() const { return Vector3(-x, -y, -z); }

	inline static float Distance(const Vector3 &a, const Vector3 &b)
	{
		return (a - b).Magnitude();
	}

	inline static float DotProduct(const Vector3 &a, const Vector3 &b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline static Vector3 CrossProduct(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
			       a.x * b.y - a.y * b.x);
	}

	inline static Vector3 Lerp(const Vector3 &a, const Vector3 &b,
				   const float value)
	{
		return (b - a) * value + a;
	}

	inline static Vector3 Zero() { return Vector3(0, 0, 0); }

	inline static Vector3 One() { return Vector3(1, 1, 1); }

	inline Vector3 Normalized() const { return *this / Magnitude(); }

	float x;
	float y;
	float z;
};

inline std::ostream &operator<<(std::ostream &o, Vector3 const &v)
{
	o << "{" << v.x << ", " << v.y << ", " << v.z << " }" << std::endl;
	return (o);
}