#pragma once

#include <Camera.hpp>
#include <Object.hpp>
#include <vector>
#include <Singleton.hpp>

class Scene : public Singleton<Scene>
{
      public:
	Scene();
	~Scene();
	std::vector<Object> objects;
	Camera camera;
};
