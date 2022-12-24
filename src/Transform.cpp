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
	angle = other.angle;
	return *this;
}
Transform::~Transform() {}
