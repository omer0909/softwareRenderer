#pragma once

#include <Light.hpp>
#include <Vector3.hpp>

class DirectionalLight : public Light
{
      public:
	DirectionalLight();
	~DirectionalLight();
	Vector3 origin;
	Vector3 dir;

	Vector3 tOrgin;
	Vector3 tDir;
	float *shadowMap;
};