#include <Quaternion.hpp>
#include <cmath>

Quaternion::Quaternion(Vector3 const &euler)
{
	const Vector3 c(std::cos(euler.z * 0.5), std::cos(euler.y * 0.5),
		  std::cos(euler.x * 0.5));
	const Vector3 s(std::sin(euler.z * 0.5), std::sin(euler.y * 0.5),
		  std::sin(euler.x * 0.5));

	w = c.x * c.y * c.z + s.x * s.y * s.z;
	i = c.x * c.y * s.z - s.x * s.y * c.z;
	k = s.x * c.y * c.z - c.x * s.y * s.z;
	j = c.x * s.y * c.z + s.x * c.y * s.z;
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

Quaternion Quaternion::operator-() const { return Quaternion(-i, -j, -k, w); }

Vector3 Quaternion::operator*(Vector3 const &v) const
{
	Quaternion result(v.x, v.y, v.z, 0);
	result = (*this * result) * -(*this);
	return Vector3(result.i, result.j, result.k);
}

Quaternion::Quaternion(Quaternion const &other) { *this = other; }

Quaternion &Quaternion::operator=(Quaternion const &other)
{
	i = other.i;
	j = other.j;
	k = other.k;
	w = other.w;
	return *this;
}

Quaternion Quaternion::Normalized()
{
	const float distance = 1.0f / std::sqrt(i * i + j * j + k * k + w * w);
	return Quaternion(i * distance, j * distance, k * distance,
			  w * distance);
}

Quaternion Quaternion::SLerp(const Quaternion &q1, const Quaternion &q2,
			     float val)
{
	Quaternion qr;

	const float dotproduct =
	    q1.i * q2.i + q1.j * q2.j + q1.k * q2.k + q1.w * q2.w;
	float theta, st, sut, sout, coeff1, coeff2;

	// val *= 0.5f;

	theta = std::acos(dotproduct);
	if (theta < 0.0)
		theta = -theta;

	st = std::sin(theta);
	sut = std::sin(val * theta);
	sout = std::sin((1 - val) * theta);
	coeff1 = sout / st;
	coeff2 = sut / st;

	qr.i = coeff1 * q1.i + coeff2 * q2.i;
	qr.j = coeff1 * q1.j + coeff2 * q2.j;
	qr.k = coeff1 * q1.k + coeff2 * q2.k;
	qr.w = coeff1 * q1.w + coeff2 * q2.w;

	return qr.Normalized();
}

Quaternion Quaternion::Lerp(const Quaternion &q1, const Quaternion &q2,
			    const float val)
{
	Quaternion qr;
	const float val_ = 1.0f - val;
	qr.i = val_ * q1.i + val * q2.i;
	qr.j = val_ * q1.j + val * q2.j;
	qr.k = val_ * q1.k + val * q2.k;
	qr.w = val_ * q1.w + val * q2.w;
	return qr.Normalized();
}

Quaternion Quaternion::operator*(float val) const
{
	Quaternion qr;
	const float val_ = 1.0f - val;
	qr.i = val * i;
	qr.j = val * j;
	qr.k = val * k;
	qr.w = val_ + val * w;
	return qr.Normalized();
}

Quaternion Quaternion::SMultiplay(float val)
{
	Quaternion qr;

	float dotproduct = 0 * i + 0 * j + 0 * k + 1 * w;
	float theta, st, sut, sout, coeff1, coeff2;

	// val *= 0.5f;

	theta = std::acos(dotproduct);
	if (theta < 0.0)
		theta = -theta;

	st = std::sin(theta);
	sut = std::sin(val * theta);
	sout = std::sin((1 - val) * theta);
	coeff1 = sout / st;
	coeff2 = sut / st;

	qr.i = coeff2 * i;
	qr.j = coeff2 * j;
	qr.k = coeff2 * k;
	qr.w = coeff1 + coeff2 * w;

	return qr.Normalized();
}

Vector3 Quaternion::ToEuler() const
{
	Vector3 angles;

	float sinr_cosp = 2 * (w * i + j * k);
	float cosr_cosp = 1 - 2 * (i * i + j * j);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = std::sqrt(1 + 2 * (w * i - j * k));
	float cosp = std::sqrt(1 - 2 * (w * i - j * k));
	angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

	float siny_cosp = 2 * (w * k + i * j);
	float cosy_cosp = 1 - 2 * (j * j + k * k);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

std::ostream &operator<<(std::ostream &o, Quaternion const &q)
{
	o << "{" << q.i << ", " << q.j << ", " << q.k << ", " << q.w << " }"
	  << std::endl;
	return (o);
}