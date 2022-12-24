#pragma once
#include <iostream>

#include <Mesh.hpp>
#include <Transform.hpp>

class Object
{
public:
	Object();
	Object(const Object &);
	Object &operator=(const Object &);
	~Object();
	Mesh mesh;
	Transform transform;
};
