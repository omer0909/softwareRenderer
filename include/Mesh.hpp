#pragma once
#include <iostream>

#include <Vector3.hpp>
#include <Vector2.hpp>

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
	unsigned int *faces_vertices;
	unsigned int *faces_uv;
	unsigned int *faces_normals;

	Vector3 *tranformed_vertices;
	Vector3 *tranformed_normals;

	unsigned int vertices_size;
	unsigned int uv_size;
	unsigned int normals_size;
	unsigned int faces_size;
};