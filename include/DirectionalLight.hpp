#pragma once

#include <Light.hpp>
#include <Object.hpp>
#include <Vector3.hpp>

class DirectionalLight : public Light
{
      public:
	DirectionalLight();
	~DirectionalLight();
	// void DirectionalLight::CalculateDirectionalLightDepth(
	//     Object const &object, unsigned int faces_start,
	//     unsigned int faces_end);
	Vector3 origin;
	Vector3 dir;

	Vector3 tOrgin;
	Vector3 tDir;
	unsigned int height;
	unsigned int with;
	float *shadowMap;
};