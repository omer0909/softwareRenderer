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
	Quaternion operator*(float val) const;
	Quaternion operator-() const;
	Quaternion SMultiplay(float val);
	Vector3 operator*(const Vector3 &a) const;
	void multiplayArray(Vector3 *src, Vector3 *dest, unsigned int length);
	~Quaternion();
	Vector3 ToEuler() const;
	float i;
	float j;
	float k;
	float w;
	static Quaternion Zero();
	static Quaternion SLerp(const Quaternion &a, const Quaternion &b,
				float val);
	static Quaternion Lerp(const Quaternion &a, const Quaternion &b,
			       const float val);

      private:
	Quaternion Normalized();
};

std::ostream &operator<<(std::ostream &o, Quaternion const &i);