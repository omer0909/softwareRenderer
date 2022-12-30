#pragma once

#include <Vector3.hpp>

class Light
{
      public:
	Light();
	~Light();
	float intensity;
	int color;
	Vector3 pos;
	Vector3 tPos;
};