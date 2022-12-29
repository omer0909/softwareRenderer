#include <Vector3.hpp>

Vector3 Vector3::Zero() { return Vector3(0, 0, 0); }

Vector3 Vector3::One() { return Vector3(1, 1, 1); }

Vector3 Vector3::operator*(const float &a) const
{
	return Vector3(x * a, y * a, z * a);
}

Vector3::Vector3() {}

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3::~Vector3() {}

Vector3::Vector3(const Vector3 &a) { *this = a; }

Vector3 &Vector3::operator=(const Vector3 &a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

Vector3 Vector3::Lerp(const Vector3 &a, const Vector3 &b, const float val)
{
	const float val2 = 1 - val;
	return {a.x * val + b.x * val2, a.y * val + b.y * val2,
		a.z * val + b.z * val2};
}

std::ostream &operator<<(std::ostream &o, Vector3 const &v)
{
	o << "{" << v.x << ", " << v.y << ", " << v.z << " }" << std::endl;
	return (o);
}

Vector3 Vector3::operator-(const Vector3 &a) const
{
	return Vector3(x - a.x, y - a.y, z - a.z);
}

Vector3 Vector3::operator+(const Vector3 &a) const
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

float Vector3::DotProduct(Vector3 const &a, Vector3 const &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::CrossProduct(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
		       a.x * b.y - a.y * b.x);
}

Vector3 Vector3::Normalized() const
{
	float factor = 1.0f / Magnitude();
	return Vector3(x * factor, y * factor, z * factor);
}

float Vector3::Magnitude() const { return sqrtf(SqrMagnitude()); }

float Vector3::SqrMagnitude() const { return (x * x) + (y * y) + (z * z); }
