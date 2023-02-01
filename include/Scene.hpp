#pragma once

#include <Camera.hpp>
#include <DirectionalLight.hpp>
#include <Image.hpp>
#include <Object.hpp>
#include <PointLight.hpp>
#include <Singleton.hpp>
#include <Window.hpp>
#include <vector>

class Scene : public Singleton<Scene> {
   public:
	Scene();
	~Scene();
	std::vector<Object> objects;
	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> directionalLights;
	Camera camera;
	Window *window;
	float deltaTime;
	Image matCap;
};
