#include <Object.hpp>

Object::Object() {}

Object::Object(const Object &other)
{
	if (this != &other)
		*this = other;
}

Object &Object::operator=(const Object &other)
{
	if (this == &other)
		return *this;
	mesh = other.mesh;
	transform = other.transform;
	return *this;
}

Object::~Object() {}