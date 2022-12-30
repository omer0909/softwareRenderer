#pragma once
#include <iostream>

#include <Vector2.hpp>
#include <Vector3.hpp>

class Mesh
{
      public:
	Mesh();
	Mesh(const Mesh &);
	Mesh &operator=(const Mesh &);
	~Mesh();

	std::string name;
	Vector3 *vertices;
	Vector2 *uv;
	Vector3 *normals;
	Vector3 *tVertices;

	unsigned int faces_size;
};