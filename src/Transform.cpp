#include <Transform.hpp>

Transform::Transform() {}
Transform::Transform(const Transform &other)
{
	if (this != &other)
		*this = other;
}
Transform &Transform::operator=(const Transform &other)
{
	if (this == &other)
		return *this;
	pos = other.pos;
	rotation = other.rotation;
	return *this;
}
Transform::~Transform() {}

Vector3 Transform::Forward() const { return this->rotation * Vector3(0, 0, 1); }
