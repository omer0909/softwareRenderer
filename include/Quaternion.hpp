#pragma once

#include <Vector3.hpp>

class Quaternion
{
      public:
	Quaternion();
	Quaternion(float i, float j, float k, float w);
	Quaternion(const Vector3 &euler);
	Quaternion(const Quaternion &);
	Quaternion &operator=(const Quaternion &);
	Quaternion operator*(const Quaternion &a) const;
	Vector3 operator*(const Vector3 &a) const;
	void multiplayArray(Vector3 *src, Vector3 *dest, unsigned int length);
	~Quaternion();
	float i;
	float j;
	float k;
	float w;
	static Quaternion Zero();
};