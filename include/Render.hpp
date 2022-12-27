#pragma once

#include <Scene.hpp>
#include <Window.hpp>
#include <limits>
#include <thread>

#define EPSILON 0.00001f
#define PI 3.1415926
#define ANGLE_TO_RADIAN (PI / 180)

const unsigned int THREAD_NUMBER = std::thread::hardware_concurrency();

struct RenderData {
	unsigned int *tIndex;
	Vector2 *uv;
	Vector3 *tNormal;
	float *zBuffer;

	unsigned int faces_start;
	unsigned int faces_end;
};

class Render
{
      private:
	Vector2 worldToScreenPoint(Vector3 const &pos);
	void RenderObject(Object const &object, RenderData &_data);
	bool controlFunctionPoint(Vector2 const &a, Vector2 const &b,
				  Vector2 const &point);
	void clear_zBuffer();
	void CalculatePixel(RenderData *data, unsigned int start,
			    unsigned int end);
	unsigned int height;
	unsigned int with;
	int halfHeight;
	int halfWith;
	float focalLegenth;

	float wallSlope;
	float ceilSlope;

      public:
	Render(Window &_window);
	Render(const Render &) = delete;
	Render &operator=(const Render &) = delete;
	~Render();
	void View();

	RenderData *data;
	std::thread *threads;
	Window &window;
};