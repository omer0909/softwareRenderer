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

Render::Render(Render const &other) : window(other.window)
{
	if (this != &other)
		*this = other;
}

Render &Render::operator=(Render const &other)
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

inline Vector3 transform(Vector3 const &pos, Transform const &transform)
{
	Transform &cam = Scene::Get().camera.transform;
	Vector3 nPos = transform.rotation * pos;
	nPos = nPos - transform.pos;
	nPos = nPos - cam.pos;
	nPos = cam.rotation * nPos;
	return {nPos};
}

inline Vector2 Render::worldToScreenPoint(Vector3 const &pos)
{
	float factor = 1.0f / (pos.z * focalLegenth);
	float x = (-pos.x * factor);
	float y = (-pos.y * factor);
	return {x + halfWith, y + halfHeight};
}

inline bool Render::controlFunctionPoint(Vector2 const &a, Vector2 const &b,
					 Vector2 const &point)
{
	Vector2 edge1 = a - b;
	Vector2 edge2 = b - point;

	return (edge1.x * edge2.y) - (edge1.y * edge2.x) > 0;
}

static inline int MinInt(int a, int b) { return (a < b) ? a : b; }
static inline int MaxInt(int a, int b) { return (a < b) ? b : a; }

int normalToColor(Vector3 const &normal)
{
	return ((int)((normal.x + 1) * 128) << 24) +
	       ((int)((normal.y + 1) * 128) << 16) +
	       ((int)((normal.z + 1) * 128) << 8) + 255;
}

inline void Render::RenderObject(Object const &object)
{
	constexpr float minView = 0.3f;

	Mesh const &mesh = object.mesh;
	for (unsigned int t = 0; t < mesh.faces_size; t += 3) {

		const Vector3 worldPos[3] = {
		    transform(mesh.vertices[mesh.faces_vertices[t]],
			      object.transform),
		    transform(mesh.vertices[mesh.faces_vertices[t + 1]],
			      object.transform),
		    transform(mesh.vertices[mesh.faces_vertices[t + 2]],
			      object.transform)};

		if (worldPos[0].z < minView && worldPos[1].z < minView &&
		    worldPos[2].z < minView)
			continue;

		const Vector3 v1v0 = worldPos[1] - worldPos[0];
		const Vector3 v2v0 = worldPos[2] - worldPos[0];

		const Vector3 normal = Vector3::CrossProduct(v1v0, v2v0);
		if (Vector3::DotProduct(worldPos[0], normal) > EPSILON)
			continue;

		const Vector3 rov0 = -worldPos[0];

		const Vector2 cTris2d[3] = {worldToScreenPoint(worldPos[0]),
					    worldToScreenPoint(worldPos[1]),
					    worldToScreenPoint(worldPos[2])};

		if ((cTris2d[0].x > with || cTris2d[0].x < 0) &&
		    (cTris2d[1].x > with || cTris2d[1].x < 0) &&
		    (cTris2d[2].x > with || cTris2d[2].x < 0))
			continue;

		const int bA[3] = {cTris2d[2].y < cTris2d[0].y,
				   cTris2d[0].y < cTris2d[1].y,
				   cTris2d[1].y < cTris2d[2].y};

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

		const int right =
		    controlFunctionPoint(tris2d[0], tris2d[2], tris2d[1]);

		const int indexs[3][2] = {{1, 0}, {2, 0}, {2, 1}};
		float inclination[3];
		float add[3];

		for (int i = 0; i < 3; i++) {
			const Vector2 a = tris2d[indexs[i][0]];
			const Vector2 b = tris2d[indexs[i][1]];
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
				const Vector3 dir =
				    Vector3(-x + halfWith, -y + halfHeight,
					    height)
					.Normalized();

				const Vector3 q =
				    Vector3::CrossProduct(rov0, dir);
				const float d =
				    1.0 / Vector3::DotProduct(dir, normal);
				const Vector2 uv(
				    d * Vector3::DotProduct(-q, v2v0),
				    d * Vector3::DotProduct(q, v1v0));
				const float distance =
				    d * Vector3::DotProduct(-normal, rov0);

				if (distance < zBuffer[y * height + x] &&
				    distance > minView) {
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

				const Vector3 dir =
				    Vector3(-x + halfWith, -y + halfHeight,
					    height)
					.Normalized();

				const Vector3 q =
				    Vector3::CrossProduct(rov0, dir);
				const float d =
				    1.0 / Vector3::DotProduct(dir, normal);
				const Vector2 uv(
				    d * Vector3::DotProduct(-q, v2v0),
				    d * Vector3::DotProduct(q, v1v0));
				const float distance =
				    d * Vector3::DotProduct(-normal, rov0);

				if (distance < zBuffer[y * height + x] &&
				    distance > minView) {
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
