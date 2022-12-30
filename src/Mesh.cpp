#include <Mesh.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

Mesh::Mesh()
{
	vertices = new Vector3[0];
	uv = new Vector2[0];
	normals = new Vector3[0];
}

Mesh::Mesh(const Mesh &a) { *this = a; }

Mesh &Mesh::operator=(const Mesh &a)
{
	if (this == &a)
		return *this;

	faces_size = a.faces_size;

	vertices = new Vector3[a.faces_size];
	uv = new Vector2[a.faces_size];
	normals = new Vector3[a.faces_size];
	tVertices = new Vector3[a.faces_size];

	std::copy(a.vertices, a.vertices + a.faces_size, vertices);
	std::copy(a.uv, a.uv + a.faces_size, uv);
	std::copy(a.normals, a.normals + a.faces_size, normals);

	return *this;
}

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] uv;
	delete[] normals;
	delete[] tVertices;
}