#pragma once

#include <Image.hpp>
#include <Scene.hpp>
#include <Window.hpp>
#include <limits>
#include <thread>

#define WIDTH 1920
#define HEIGHT 1080

#define EPSILON 0.00001f
#define ANGLE_TO_RADIAN (M_PI / 180)

const unsigned int THREAD_NUMBER = std::thread::hardware_concurrency();

struct TrisData {
	Vector3 normals[3];
	Vector2 *uv;
};

struct RenderData {
	Vector2 *tuv;
	float *zBuffer;

	TrisData *trisData;

	unsigned int faces_start;
	unsigned int faces_end;
};

class Render {
   private:
	Vector2 worldToScreenPoint(Vector3 const &pos);
	void RenderObject(Object const &object, RenderData &_data);
	bool controlFunctionPoint(Vector2 const &a, Vector2 const &b, Vector2 const &point);
	void clear_zBuffer();
	void CalculatePixel(RenderData *data, unsigned int start, unsigned int end);
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
	Vector3 *pixelDir;
};