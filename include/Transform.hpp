#pragma once

#include <Quaternion.hpp>
#include <Vector3.hpp>

class Transform
{
      public:
	Transform();
	Transform(const Transform &);
	Transform &operator=(const Transform &);
	Vector3 Forward() const;
	Vector3 Right() const;
	Vector3 Up() const;
	~Transform();
	Vector3 pos;
	Quaternion rotation;
};