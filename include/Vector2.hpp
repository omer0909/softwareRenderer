#pragma once

#include <cmath>
#include <iostream>

class Vector2
{
      public:
	inline Vector2() {}

	inline Vector2(float _x, float _y) : x(_x), y(_y) {}

	inline Vector2(const Vector2 &a) { *this = a; }

	inline Vector2 &operator=(const Vector2 &a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}

	inline float Magnitude() const { return std::sqrt(SqrMagnitude()); }

	inline float SqrMagnitude() const { return x * x + y * y; }

	inline Vector2 operator+(const Vector2 &a) const
	{
		return Vector2(x + a.x, y + a.y);
	}

	inline Vector2 operator-(const Vector2 &a) const
	{
		return Vector2(x - a.x, y - a.y);
	}

	inline Vector2 operator*(const Vector2 &a) const
	{
		return Vector2(x * a.x, y * a.y);
	}

	inline Vector2 operator/(const Vector2 &a) const
	{
		return Vector2(x / a.x, y / a.y);
	}

	inline Vector2 operator*(const float &a) const
	{
		return Vector2(x * a, y * a);
	}

	inline Vector2 operator/(const float &a) const
	{
		float factor = 1.0f / a;
		return Vector2(x * factor, y * factor);
	}

	inline Vector2 Normalized() const { return *this / Magnitude(); }

	inline static float Distance(const Vector2 &a, const Vector2 &b)
	{
		return (a - b).Magnitude();
	}

	inline static float DotProduct(const Vector2 &a, const Vector2 &b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline static Vector2 Lerp(const Vector2 &a, const Vector2 &b, float value)
	{
		return (b - a) * value + a;
	}

	inline static Vector2 Zero() { return Vector2(0, 0); }

	inline static Vector2 One() { return Vector2(1, 1); }

	inline static Vector2 Up() { return Vector2(0, 1); }

	inline static Vector2 Down() { return Vector2(0, -1); }

	inline static Vector2 Right() { return Vector2(1, 0); }

	inline static Vector2 Left() { return Vector2(-1, 0); }

	float x;
	float y;
};

inline std::ostream &operator<<(std::ostream &o, Vector2 const &v)
{
	o << "{" << v.x << ", " << v.y << "}" << std::endl;
	return (o);
}