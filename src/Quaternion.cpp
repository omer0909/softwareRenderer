#include <Quaternion.hpp>
#include <cmath>

Quaternion::Quaternion(Vector3 const &euler)
{
	Vector3 c(cosf(euler.z * 0.5), cosf(euler.y * 0.5),
		  cosf(euler.x * 0.5));
	Vector3 s(sinf(euler.z * 0.5), sinf(euler.y * 0.5),
		  sinf(euler.x * 0.5));

	w = c.x * c.y * c.z + s.x * s.y * s.z;
	i = s.x * c.y * c.z - c.x * s.y * s.z;
	j = c.x * s.y * c.z + s.x * c.y * s.z;
	k = c.x * c.y * s.z - s.x * s.y * c.z;
}

Quaternion::Quaternion(float _i, float _j, float _k, float _w)
    : i(_i), j(_j), k(_k), w(_w)
{
}

Quaternion Quaternion::operator*(Quaternion const &a) const
{
	Quaternion qr;

	qr.i = (w * a.i) + (i * a.w) + (j * a.k) - (k * a.j);
	qr.j = (w * a.j) - (i * a.k) + (j * a.w) + (k * a.i);
	qr.k = (w * a.k) + (i * a.j) - (j * a.i) + (k * a.w);
	qr.w = (w * a.w) - (i * a.i) - (j * a.j) - (k * a.k);
	return (qr);
}

static Quaternion QuaternionConjugate(Quaternion const &q)
{
	return Quaternion(-q.i, -q.j, -q.k, q.w);
}

Vector3 Quaternion::operator*(Vector3 const &v) const
{
	Quaternion result(v.x, v.y, v.z, 0);
	result = (*this * result) * QuaternionConjugate(*this);
	return Vector3(result.i, result.j, result.k);
}

Quaternion::Quaternion() {}

Quaternion::Quaternion(Quaternion const &other) { *this = other; }

Quaternion &Quaternion::operator=(Quaternion const &other)
{
	i = other.i;
	j = other.j;
	k = other.k;
	w = other.w;
	return *this;
}

Quaternion::~Quaternion() {}

Quaternion Quaternion::Zero() { return Quaternion(0, 0, 0, 1); }