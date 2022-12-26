#pragma once

#include <Scene.hpp>
#include <Window.hpp>
#include <limits>

#define EPSILON 0.00001f
#define PI 3.1415926
#define ANGLE_TO_RADIAN (PI / 180)

class Render
{
      private:
	Vector2 worldToScreenPoint(Vector3 const &pos);
	void RenderObject(Object const &object);
	bool controlFunctionPoint(Vector2 const &a, Vector2 const &b,
				  Vector2 const &point);
	void clear_zBuffer();
	int height;
	int with;
	int halfHeight;
	int halfWith;
	float focalLegenth;

      public:
	Render(Window &_window);
	Render(const Render &);
	Render &operator=(const Render &);
	~Render();
	void View();

	static unsigned int thred_number;
	float *zBuffer;
	Window &window;
};