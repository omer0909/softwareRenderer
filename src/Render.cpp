#include <Render.hpp>

Render::Render(Window &_window) : window(_window)
{
	with = window.GetXSize();
	height = window.GetYSize();
	zBuffer = new float[with * height];
	focalLegenth = 1.0f / (height * 1);
	halfHeight = height / 2;
	halfWith = with / 2;
}

Render::Render(const Render &other) : window(other.window)
{
	if (this != &other)
		*this = other;
}

Render &Render::operator=(const Render &other)
{
	window = other.window;
	std::copy(other.zBuffer, other.zBuffer + with * height, zBuffer);
	window = other.window;
	halfHeight = other.halfHeight;
	halfWith = other.halfWith;
	focalLegenth = other.focalLegenth;
	return *this;
}

Render::~Render() { delete zBuffer; }

inline Vector3 transform(Vector3 pos, Transform transform)
{

	Transform &cam = Scene::Get().camera.transform;
	pos = transform.rotation * pos;
	pos = pos - transform.pos;
	pos = pos - cam.pos;
	pos = cam.rotation * pos;
	return {pos};
}

inline Vector2 Render::worldToScreenPoint(Vector3 pos)
{
	float x = (-pos.x / (pos.z * focalLegenth));
	float y = (-pos.y / (pos.z * focalLegenth));
	return {x + halfWith, y + halfHeight};
}

inline bool Render::controlFunctionPoint(Vector2 a, Vector2 b, Vector2 point)
{
	Vector2 edge1 = a - b;
	Vector2 edge2 = b - point;

	return (edge1.x * edge2.y) - (edge1.y * edge2.x) > 0;
}

static inline int MinInt(int a, int b) { return (a < b) ? a : b; }
static inline int MaxInt(int a, int b) { return (a < b) ? b : a; }

int normalToColor(Vector3 normal)
{
	return ((int)((normal.x + 1) * 128) << 24) +
	       ((int)((normal.y + 1) * 128) << 16) +
	       ((int)((normal.z + 1) * 128) << 8) + 255;
}

void TriIntersect(const Vector3 dir, const Vector3 vertex[3], Vector2 &uv,
		  float &distance)
{
	Vector3 v1v0 = vertex[1] - vertex[0];
	Vector3 v2v0 = vertex[2] - vertex[0];
	Vector3 rov0 = -vertex[0];

	Vector3 n = Vector3::CrossProduct(v1v0, v2v0);
	Vector3 q = Vector3::CrossProduct(rov0, dir);
	float d = 1.0 / Vector3::DotProduct(dir, n);
	uv.x = d * Vector3::DotProduct(-q, v2v0);
	uv.y = d * Vector3::DotProduct(q, v1v0);
	distance = d * Vector3::DotProduct(-n, rov0);

	if (uv.x < 0.0 || uv.y < 0.0 || (uv.x + uv.y) > 1.0)
		distance = -1.0;
}

inline void Render::RenderObject(Object object)
{
	Mesh &mesh = object.mesh;
	for (unsigned int t = 0; t < mesh.faces_size; t += 3) {

		const Vector3 worldPos[3] = {
		    transform(mesh.vertices[mesh.faces_vertices[t]],
			      object.transform),
		    transform(mesh.vertices[mesh.faces_vertices[t + 1]],
			      object.transform),
		    transform(mesh.vertices[mesh.faces_vertices[t + 2]],
			      object.transform)};

		const Vector2 cTris2d[3] = {worldToScreenPoint(worldPos[0]),
					    worldToScreenPoint(worldPos[1]),
					    worldToScreenPoint(worldPos[2])};

		if (controlFunctionPoint(cTris2d[0], cTris2d[1], cTris2d[2]))
			continue;

		const int bA[3] = {cTris2d[2].y < cTris2d[0].y,
				   cTris2d[0].y < cTris2d[1].y,
				   cTris2d[1].y < cTris2d[2].y};

		if (!bA[0] && !bA[1] && !bA[2])
			continue;

		const int sIndex[3] = {bA[0] + !bA[1], bA[1] + !bA[2],
				       bA[2] + !bA[0]};

		Vector2 tris2d[3];
		tris2d[sIndex[0]] = cTris2d[0];
		tris2d[sIndex[1]] = cTris2d[1];
		tris2d[sIndex[2]] = cTris2d[2];

		if (tris2d[2].y < 0 || tris2d[0].y > height)
			continue;

		Vector3 tNormals[3];
		tNormals[sIndex[0]] = mesh.normals[mesh.faces_normals[t]];
		tNormals[sIndex[1]] = mesh.normals[mesh.faces_normals[t + 1]];
		tNormals[sIndex[2]] = mesh.normals[mesh.faces_normals[t + 2]];

		int right =
		    controlFunctionPoint(tris2d[0], tris2d[2], tris2d[1]);

		const int indexs[3][2] = {{1, 0}, {2, 0}, {2, 1}};
		float inclination[3];
		float add[3];

		for (int i = 0; i < 3; i++) {
			Vector2 a = tris2d[indexs[i][0]];
			Vector2 b = tris2d[indexs[i][1]];
			if (a.y == b.y)
				continue;

			inclination[i] = (b.x - a.x) / (b.y - a.y);
			add[i] = a.x - a.y * inclination[i];
		}

		const int aMaxY = MinInt(tris2d[1].y, height - 1);
		for (int y = MaxInt(0, tris2d[0].y + 1); y <= aMaxY; y++) {
			const int a = right;
			const int b = !right;

			const float max = y * inclination[a] + add[a];
			const float min = y * inclination[b] + add[b];

			const int aMaxX = MinInt(max, with - 1);
			for (int x = MaxInt(0, min + 1); x <= aMaxX; x++) {
				float distance;
				Vector2 uv;
				TriIntersect(Vector3(-x + halfWith,
						     -y + halfHeight, height)
						 .Normalized(),
					     worldPos, uv, distance);
				if (distance < zBuffer[y * height + x]) {
					zBuffer[y * height + x] = distance;
					window.SetPixel(
					    x, y,
					    normalToColor(Vector3::One() *
							  distance * 10));
				}
			}
		}

		const int bMaxY = MinInt(tris2d[2].y, height - 1);
		for (int y = MaxInt(0, tris2d[1].y + 1); y <= bMaxY; y++) {
			const int a = right + 1;
			const int b = !right + 1;

			const float max = y * inclination[b] + add[b];
			const float min = y * inclination[a] + add[a];

			const int bMaxX = MinInt(max, with - 1);
			for (int x = MaxInt(0, min + 1); x <= bMaxX; x++) {
				float distance;
				Vector2 uv;
				TriIntersect(Vector3(-x + halfWith,
						     -y + halfHeight, height)
						 .Normalized(),
					     worldPos, uv, distance);
				if (distance < zBuffer[y * height + x]) {
					zBuffer[y * height + x] = distance;
					window.SetPixel(
					    x, y,
					    normalToColor(Vector3::One() *
							  distance * 10));
				}
			}
		}
	}
}

void Render::clear_zBuffer()
{
	for (int i = 0; i < with * height; i++)
		zBuffer[i] = std::numeric_limits<float>::max();
}

void Render::View()
{
	Scene &scene = Scene::Get();
	clear_zBuffer();
	window.Clear(0);
	for (unsigned int i = 0; i < scene.objects.size(); i++) {
		RenderObject(scene.objects[i]);
	}
	window.UpdateSurface();
}
