#pragma once

#include <Camera.hpp>
#include <Object.hpp>
#include <vector>
#include <Singleton.hpp>
#include <Window.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>

class Scene : public Singleton<Scene>
{
      public:
	Scene();
	~Scene();
	std::vector<Object> objects;
	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> directionalLights;
	Camera camera;
	Window *window;
	float deltaTime;
};
