#pragma once

#include <Transform.hpp>

class Camera
{
      public:
	Camera();
	~Camera();
	Transform transform;

	static constexpr float minView = 0.3f;
	static constexpr float maxView = 10000.0f;
	static constexpr float shadowDistance = 10;
};
