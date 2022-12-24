#include <iostream>
#include <algorithm>
#include <iterator>
#include <Mesh.hpp>

Mesh::Mesh()
{
	vertices = new Vector3[0];
	uv = new Vector2[0];
	normals = new Vector3[0];
	faces_vertices = new unsigned int[0];
	faces_uv = new unsigned int[0];
	faces_normals = new unsigned int[0];
}

Mesh::Mesh(const Mesh &a)
{
	*this = a;
}

Mesh &Mesh::operator=(const Mesh &a)
{
	if (this == &a)
		return *this;
	// delete[] vertices;
	// delete[] uv;
	// delete[] normals;
	// delete[] faces_vertices;
	// delete[] faces_uv;
	// delete[] faces_normals;
	// delete[] tranformed_vertices;
	// delete[] tranformed_normals;

	vertices_size = a.vertices_size;
	uv_size = a.uv_size;
	normals_size = a.normals_size;
	faces_size = a.faces_size;

	vertices = new Vector3[a.vertices_size];
	uv = new Vector2[a.uv_size];
	normals = new Vector3[a.normals_size];
	faces_vertices = new unsigned int[a.faces_size];
	faces_uv = new unsigned int[a.faces_size];
	faces_normals = new unsigned int[a.faces_size];

	std::copy(a.vertices, a.vertices + a.vertices_size, vertices);
	std::copy(a.uv, a.uv + a.uv_size, uv);
	std::copy(a.normals, a.normals + a.normals_size, normals);
	std::copy(a.faces_vertices, a.faces_vertices + a.faces_size, faces_vertices);
	std::copy(a.faces_uv, a.faces_uv + a.faces_size, faces_uv);
	std::copy(a.faces_normals, a.faces_normals + a.faces_size, faces_normals);
	return *this;
}

Mesh::~Mesh()
{
	// delete[] vertices;
	// delete[] uv;
	// delete[] normals;
	// delete[] faces_vertices;
	// delete[] faces_uv;
	// delete[] faces_normals;
}