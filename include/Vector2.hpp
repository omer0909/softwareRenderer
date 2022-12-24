#pragma once

#include <iostream>

class Vector2
{
public:
	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2 &);
	Vector2 &operator=(const Vector2 &);
	~Vector2();
	float x;
	float y;
	float magnitude();
	Vector2 operator+(const Vector2 &a) const;
	Vector2 operator-(const Vector2 &a) const;
	Vector2 operator*(const Vector2 &a) const;
	Vector2 operator/(const Vector2 &a) const;
	Vector2 operator*(const float &a) const;
	Vector2 operator/(const float &a) const;
	static float Distance(const Vector2 &a, const Vector2 &b);
	static float DotProduct(const Vector2 &a, const Vector2 &b);
};

std::ostream &operator<<(std::ostream &o, Vector2 const &i);