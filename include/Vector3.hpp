#pragma once

#include <iostream>

class Vector3
{
      public:
	Vector3();
	Vector3(float _x, float _y, float _z);
	Vector3(const Vector3 &);
	Vector3 &operator=(const Vector3 &);
	~Vector3();
	float x;
	float y;
	float z;
	float magnitude();
	Vector3 operator+(const Vector3 &a) const;
	Vector3 operator-(const Vector3 &a) const;
	Vector3 operator*(const Vector3 &a) const;
	Vector3 operator/(const Vector3 &a) const;
	Vector3 operator*(const float &a) const;
	Vector3 operator/(const float &a) const;
	Vector3 operator-();
	static float Distance(const Vector3 &a, const Vector3 &b);
	static float DotProduct(const Vector3 &a, const Vector3 &b);
	static Vector3 CrossProduct(const Vector3 &a, const Vector3 &b);
	static Vector3 Lerp(const Vector3 &a, const Vector3 &b,
			    const float val);
	static Vector3 Zero();
	static Vector3 One();
};

std::ostream &operator<<(std::ostream &o, Vector3 const &i);