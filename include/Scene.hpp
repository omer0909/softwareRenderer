#pragma once

#include <Camera.hpp>
#include <Object.hpp>
#include <vector>
#include <Singleton.hpp>
#include <Window.hpp>
#include <Light.hpp>

class Scene : public Singleton<Scene>
{
      public:
	Scene();
	~Scene();
	std::vector<Object> objects;
	std::vector<Light> lights;
	Camera camera;
	Window *window;
	float deltaTime;
};
