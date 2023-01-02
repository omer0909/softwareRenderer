#pragma once

#include <Light.hpp>
#include <Vector3.hpp>

class PointLight : public Light
{
      public:
	PointLight();
	~PointLight();
	Vector3 pos;
	Vector3 tPos;
};