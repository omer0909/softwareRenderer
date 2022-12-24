#pragma once

#include <Quaternion.hpp>
#include <Vector3.hpp>

class Transform
{
      public:
	Transform();
	Transform(const Transform &);
	Transform &operator=(const Transform &);
	~Transform();
	Vector3 pos;
	Quaternion angle;
};