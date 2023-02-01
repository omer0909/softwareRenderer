#pragma once

#include <Vector3.hpp>

class Quaternion {
   public:
	inline Quaternion() {}
	inline Quaternion(float _i, float _j, float _k, float _w)
	    : i(_i), j(_j), k(_k), w(_w) {
	}

	Quaternion(const Vector3 &euler);
	Quaternion(const Quaternion &);
	Quaternion &operator=(const Quaternion &);
	Quaternion operator*(const Quaternion &a) const;
	Quaternion operator*(float val) const;
	Quaternion operator-() const;
	Quaternion SMultiplay(float val);
	Vector3 operator*(const Vector3 &a) const;
	void multiplayArray(Vector3 *src, Vector3 *dest, unsigned int length);
	inline ~Quaternion() {}
	Vector3 ToEuler() const;
	float i;
	float j;
	float k;
	float w;
	inline static Quaternion Zero() { return Quaternion(0, 0, 0, 1); }
	static Quaternion SLerp(const Quaternion &a, const Quaternion &b, float val);
	static Quaternion Lerp(const Quaternion &a, const Quaternion &b, const float val);

   private:
	Quaternion Normalized();
};

std::ostream &operator<<(std::ostream &o, Quaternion const &i);