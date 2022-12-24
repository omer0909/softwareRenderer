#include <Vector3.hpp>

Vector3::Vector3() {}

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3::~Vector3() {}

Vector3::Vector3(const Vector3 &a) { *this = a; }

Vector3 &Vector3::operator=(const Vector3 &a)
{
	if (this == &a)
		return *this;
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