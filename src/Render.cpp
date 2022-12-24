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

inline Vector3 transform(Vector3 pos)
{
	Transform &cam = Scene::Get().camera.transform;

	return {pos.x - cam.pos.x, pos.y - cam.pos.y, pos.z - cam.pos.z};
}

inline Vector2 Render::worldToScreenPoint(Vector3 pos)
{
	float x = (-pos.x / (pos.z * focalLegenth));
	float y = (-pos.y / (pos.z * focalLegenth));
	return {x + halfWith, y + halfHeight};
}

bool Render::controlFunctionPoint(Vector2 a, Vector2 b, Vector2 point)
{
	float tolorance = a.x - b.x;
	if (tolorance < EPSILON && tolorance > -EPSILON)
		return ((a.x - point.x) > 0) == (a.y < b.y);

	float inclination = (b.y - a.y) / (b.x - a.x);
	float add = a.y - (a.x * inclination);

	return ((point.x * inclination) + add < point.y) == (a.x < b.x);
}

static inline int MinInt(int a, int b) { return (a < b) ? a : b; }
static inline int MaxInt(int a, int b) { return (a < b) ? b : a; }

int normalToColor(Vector3 normal)
{
	return ((int)((normal.x + 1) * 128) << 24) + ((int)((normal.y + 1) * 128) << 16) +
	       ((int)((normal.z + 1) * 128) << 8) + 255;
}

inline void Render::RenderObject(Object object)
{
	Mesh &mesh = object.mesh;
	for (unsigned int t = 0; t < mesh.faces_size; t += 3) {

		const Vector3 worldPos[3] = {
		    transform(mesh.vertices[mesh.faces_vertices[t]]),
		    transform(mesh.vertices[mesh.faces_vertices[t + 1]]),
		    transform(mesh.vertices[mesh.faces_vertices[t + 2]])};

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

		float zPos[3];
		zPos[sIndex[0]] = worldPos[0].z;
		zPos[sIndex[1]] = worldPos[1].z;
		zPos[sIndex[2]] = worldPos[2].z;

		Vector3 tNormals[3];
		tNormals[sIndex[0]] = mesh.normals[mesh.faces_normals[t]];
		tNormals[sIndex[1]] = mesh.normals[mesh.faces_normals[t + 1]];
		tNormals[sIndex[2]] = mesh.normals[mesh.faces_normals[t + 2]];

		int right =
		    controlFunctionPoint(tris2d[0], tris2d[2], tris2d[1]);

		const int indexs[3][2] = {{1, 0}, {2, 0}, {2, 1}};
		float inclination[3];
		float add[3];
		float lerp[3];

		for (int i = 0; i < 3; i++) {
			Vector2 a = tris2d[indexs[i][0]];
			Vector2 b = tris2d[indexs[i][1]];
			if (a.y == b.y)
				continue;

			lerp[i] = 1 / (a.y - b.y);
			inclination[i] = (b.x - a.x) / (b.y - a.y);
			add[i] = a.x - a.y * inclination[i];
		}

		const int aMaxY = MinInt(tris2d[1].y, height - 1);
		for (int y = MaxInt(0, tris2d[0].y + 1); y <= aMaxY; y++) {
			const int a = right;
			const int b = !right;

			const float max = y * inclination[a] + add[a];
			const float min = y * inclination[b] + add[b];

			const float aL = (y - tris2d[0].y) * lerp[b];
			const float bL = (y - tris2d[0].y) * lerp[a];

			const float zPosA =
			    zPos[a + 1] * bL + zPos[0] * (1.0f - bL);
			const float zPosB =
			    zPos[b + 1] * aL + zPos[0] * (1.0f - aL);

			const Vector3 aNormal =
			    Vector3::Lerp(tNormals[a + 1], tNormals[0], bL);
			const Vector3 bNormal =
			    Vector3::Lerp(tNormals[b + 1], tNormals[0], aL);
			const float cL = 1.0f / (max - min);
			const int aMaxX = MinInt(max, with - 1);
			for (int x = MaxInt(0, min + 1); x <= aMaxX; x++) {
				const float rZPos =
				    zPosA * (x - min) * cL +
				    zPosB * (1.0f - (x - min) * cL);
				if (rZPos < zBuffer[y * height + x]) {
					const Vector3 rNormal = Vector3::Lerp(
					    aNormal, bNormal, (x - min) * cL);
					zBuffer[y * height + x] = rZPos;
					window.SetPixel(x, y,
							normalToColor(rNormal));
				}
			}
		}

		const int bMaxY = MinInt(tris2d[2].y, height - 1);
		for (int y = MaxInt(0, tris2d[1].y + 1); y <= bMaxY; y++) {
			const int a = right + 1;
			const int b = !right + 1;

			const float max = y * inclination[b] + add[b];
			const float min = y * inclination[a] + add[a];

			const float aL =
			    (y - tris2d[1].y +
			     ((tris2d[1].y - tris2d[0].y) * !right)) *
			    lerp[a];
			const float bL =
			    (y - tris2d[1].y +
			     ((tris2d[1].y - tris2d[0].y) * right)) *
			    lerp[b];

			const float zPosA =
			    zPos[2] * bL + zPos[!right] * (1.0f - bL);
			const float zPosB =
			    zPos[2] * aL + zPos[right] * (1.0f - aL);

			const Vector3 aNormal =
			    Vector3::Lerp(tNormals[2], tNormals[!right], bL);
			const Vector3 bNormal =
			    Vector3::Lerp(tNormals[2], tNormals[right], aL);
			const float cL = 1.0f / (max - min);
			const int bMaxX = MinInt(max, with - 1);
			for (int x = MaxInt(0, min + 1); x <= bMaxX; x++) {
				const float rZPos =
				    zPosA * (x - min) * cL +
				    zPosB * (1.0f - (x - min) * cL);
				if (rZPos < zBuffer[y * height + x]) {
					const Vector3 rNormal = Vector3::Lerp(
					    aNormal, bNormal, (x - min) * cL);
					zBuffer[y * height + x] = rZPos;
					window.SetPixel(x, y,
							normalToColor(rNormal));
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
