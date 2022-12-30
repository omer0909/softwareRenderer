#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include <obj_read.hpp>

using namespace std;

template <typename T> void listCopyToArray(list<T> &src, T *&arr)
{
	delete[] arr;
	arr = new T[src.size()];
	int i = 0;
	for (T const &a : src) {
		arr[i++] = a;
	}
}

Mesh obj_read(std::string name)
{
	fstream file;

	file.open(name, ios_base::in);
	if (file.fail()) {
		cerr << name << " file can not read" << std::endl;
		return Mesh();
	}
	list<Vector3> vertices;
	list<Vector2> uv;
	list<Vector3> normals;
	list<unsigned int> faces_vertices;
	list<unsigned int> faces_uv;
	list<unsigned int> faces_normals;

	string line;
	// verices
	while (line.compare(0, 2, "v "))
		getline(file, line);
	while (!line.compare(0, 2, "v ")) {
		stringstream ssline(line);
		string tmp;
		ssline >> tmp;
		ssline >> tmp;
		float x = strtof(tmp.c_str(), NULL);
		ssline >> tmp;
		float y = strtof(tmp.c_str(), NULL);
		ssline >> tmp;
		float z = strtof(tmp.c_str(), NULL);
		vertices.push_back(Vector3(x, y, z));
		getline(file, line);
	}
	// uv
	while (line.compare(0, 3, "vt "))
		getline(file, line);
	while (!line.compare(0, 3, "vt ")) {
		stringstream ssline(line);
		string tmp;
		ssline >> tmp;
		ssline >> tmp;
		float x = strtof(tmp.c_str(), NULL);
		ssline >> tmp;
		float y = strtof(tmp.c_str(), NULL);
		uv.push_back(Vector2(x, y));
		getline(file, line);
	}
	// narmals
	while (line.compare(0, 3, "vn "))
		getline(file, line);
	while (!line.compare(0, 3, "vn ")) {
		stringstream ssline(line);
		string tmp;
		ssline >> tmp;
		ssline >> tmp;
		float x = strtof(tmp.c_str(), NULL);
		ssline >> tmp;
		float y = strtof(tmp.c_str(), NULL);
		ssline >> tmp;
		float z = strtof(tmp.c_str(), NULL);
		normals.push_back(Vector3(x, y, z));
		getline(file, line);
	}
	// faces
	while (line.compare(0, 2, "f "))
		getline(file, line);
	while (!line.compare(0, 2, "f ")) {
		stringstream ssline(line);
		string tmp;
		ssline >> tmp;
		for (int i = 0; i < 3; i++) {
			getline(ssline, tmp, '/');
			faces_vertices.push_back(stoi(tmp) - 1);
			getline(ssline, tmp, '/');
			faces_uv.push_back(stoi(tmp) - 1);
			getline(ssline, tmp, ' ');
			faces_normals.push_back(stoi(tmp) - 1);
		}
		getline(file, line);
	}

	file.close();

	Mesh result;

	listCopyToArray<Vector3>(vertices, result.vertices);
	result.uv = new Vector2[vertices.size()];
	result.normals = new Vector3[vertices.size()];

	Vector3 *tmpVertices = new Vector3[0];
	Vector2 *tmpUV = new Vector2[0];
	Vector3 *tmpNormals = new Vector3[0];

	listCopyToArray<Vector3>(vertices, tmpVertices);
	listCopyToArray<Vector2>(uv, tmpUV);
	listCopyToArray<Vector3>(normals, tmpNormals);

	result.faces_size = faces_vertices.size();

	result.vertices = new Vector3[faces_vertices.size()];
	int i = 0;
	for (auto &val : faces_vertices) {
		result.vertices[i++] = tmpVertices[val];
	}

	result.uv = new Vector2[faces_uv.size()];
	i = 0;
	for (auto &val : faces_uv) {
		result.uv[i++] = tmpUV[val];
	}

	result.normals = new Vector3[faces_normals.size()];
	i = 0;
	for (auto &val : faces_normals) {
		result.normals[i++] = tmpNormals[val];
	}

	delete[] tmpVertices;
	delete[] tmpUV;
	delete[] tmpNormals;

	result.tVertices = new Vector3[result.faces_size];

	return result;
}