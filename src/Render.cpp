#include <Render.hpp>

void AllocateRenderData(RenderData &data, unsigned int size)
{
	data.trisData = new TrisData[size];
	data.tuv = new Vector2[size];
	data.zBuffer = new float[size];
	data.trisData = new TrisData[size];
}

void DeleteRenderData(RenderData &data)
{
	delete[] data.trisData;
	delete[] data.tuv;
	delete[] data.zBuffer;
	delete[] data.trisData;
}

Render::Render(Window &_window) : window(_window)
{
	with = window.GetXSize();
	height = window.GetYSize();
	focalLegenth = 1.0f / (height * 1);
	halfHeight = height / 2;
	halfWith = with / 2;

	data = new RenderData[THREAD_NUMBER];
	for (unsigned int i = 0; i < THREAD_NUMBER; i++) {
		AllocateRenderData(data[i], with * height);
	}
	threads = new std::thread[THREAD_NUMBER];

	clear_zBuffer();
}

Render::~Render()
{
	for (unsigned int i = 0; i < THREAD_NUMBER; i++) {
		DeleteRenderData(data[i]);
	}
}

inline Vector3 transform(Vector3 const &pos, Transform const &transform)
{
	Transform &cam = Scene::Get().camera.transform;
	Vector3 nPos = transform.rotation * pos;
	nPos = nPos - transform.pos;
	nPos = nPos - cam.pos;
	nPos = cam.rotation * nPos;
	return nPos;
}

inline Vector2 Render::worldToScreenPoint(Vector3 const &pos)
{
	float factor = 1.0f / (pos.z * focalLegenth);

	if (pos.z < EPSILON && pos.z > -EPSILON)
		factor = 0xFFFFFF;

	if (pos.z < 0)
		factor = -factor;
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

static inline float MinFloat(float a, float b) { return (a < b) ? a : b; }
static inline float MaxFloat(float a, float b) { return (a < b) ? b : a; }

void Render::RenderObject(Object const &object, RenderData &_data)
{
	Mesh const &mesh = object.mesh;

	Quaternion rotation_normal =
	    Scene::Get().camera.transform.rotation * object.transform.rotation;

	for (unsigned int t = _data.faces_start; t < _data.faces_end; t += 3) {

		mesh.tVertices[t] =
		    transform(mesh.vertices[t], object.transform);
		mesh.tVertices[t + 1] =
		    transform(mesh.vertices[t + 1], object.transform);
		mesh.tVertices[t + 2] =
		    transform(mesh.vertices[t + 2], object.transform);

		const Vector3 *worldPos = mesh.tVertices + t;

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
				const unsigned int index = y * with + x;

				const Vector3 dir =
				    Vector3(halfWith - x, halfHeight - y,
					    height)
					.Normalized();

				const Vector3 q =
				    Vector3::CrossProduct(rov0, dir);
				const float d =
				    1.0 / Vector3::DotProduct(dir, normal);
				const Vector2 tuv(
				    d * Vector3::DotProduct(-q, v2v0),
				    d * Vector3::DotProduct(q, v1v0));
				const float distance =
				    d * Vector3::DotProduct(-normal, rov0);

				if (distance < _data.zBuffer[index] &&
				    distance > minView) {

					_data.zBuffer[index] = distance;

					_data.trisData[index].normals[0] =
					    rotation_normal * mesh.normals[t];

					_data.trisData[index].normals[1] =
					    rotation_normal *
					    mesh.normals[t + 1];

					_data.trisData[index].normals[2] =
					    rotation_normal *
					    mesh.normals[t + 2];

					_data.trisData[index].uv = mesh.uv + t;

					_data.tuv[index] = tuv;
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
				const unsigned int index = y * with + x;

				const Vector3 dir =
				    Vector3(halfWith - x, halfHeight - y,
					    height)
					.Normalized();

				const Vector3 q =
				    Vector3::CrossProduct(rov0, dir);
				const float d =
				    1.0 / Vector3::DotProduct(dir, normal);
				const Vector2 tuv(
				    d * Vector3::DotProduct(-q, v2v0),
				    d * Vector3::DotProduct(q, v1v0));
				const float distance =
				    d * Vector3::DotProduct(-normal, rov0);

				if (distance < _data.zBuffer[index] &&
				    distance > minView) {

					_data.zBuffer[index] = distance;

					{
						_data.trisData[index]
						    .normals[0] =
						    rotation_normal *
						    mesh.normals[t];

						_data.trisData[index]
						    .normals[1] =
						    rotation_normal *
						    mesh.normals[t + 1];

						_data.trisData[index]
						    .normals[2] =
						    rotation_normal *
						    mesh.normals[t + 2];
					}

					_data.trisData[index].uv = mesh.uv + t;

					_data.tuv[index] = tuv;
				}
			}
		}
	}
}

void Render::clear_zBuffer()
{
	unsigned int size = with * height;
	for (unsigned int i = 0; i < THREAD_NUMBER; i++) {
		for (unsigned int j = 0; j < size; j++)
			data[i].zBuffer[j] = std::numeric_limits<float>::max();
	}
}

static inline int RoundToInt(float value)
{
	value = value + 0.5 - (value < 0);
	return static_cast<int>(value);
}

int normalToColor(Vector3 const &normal)
{
	return ((int)((normal.x + 1) * 127) +
		((int)((normal.y + 1) * 127) << 16) +
		((int)((normal.z + 1) * 127) << 8)) |
	       0xFF000000;
}

inline static Vector3 TrisLerp(const Vector3 *v, Vector2 const &uv)
{
	return v[1] * uv.x + v[2] * uv.y + v[0] * (1 - (uv.x + uv.y));
}

inline Vector3 camTransform(Vector3 const &pos)
{
	Transform &cam = Scene::Get().camera.transform;
	Vector3 nPos = pos - cam.pos;
	nPos = cam.rotation * nPos;
	return nPos;
}

bool IntersectTriangle(Vector3 const &orig, Vector3 const &dir,
		       const Vector3 *tris, float &distance, Vector2 &uv)
{

	const Vector3 edge1 = tris[1] - tris[0];
	const Vector3 edge2 = tris[2] - tris[0];

	const Vector3 pvec = Vector3::CrossProduct(dir, edge2);

	const float det = Vector3::DotProduct(edge1, pvec);

	const Vector3 tvec = orig - tris[0];
	const float inv_det = 1.0 / det;

	const Vector3 qvec = Vector3::CrossProduct(tvec, edge1);

	if (det > EPSILON) {
		uv.x = Vector3::DotProduct(tvec, pvec);
		if (uv.x < 0.0f || uv.x > det)
			return false;

		uv.y = Vector3::DotProduct(dir, qvec);
		if (uv.y < 0.0f || uv.x + uv.y > det)
			return false;

	} else if (det < -EPSILON) {
		uv.x = Vector3::DotProduct(tvec, pvec);
		if (uv.x > 0.0 || uv.x < det)
			return false;

		uv.y = Vector3::DotProduct(dir, qvec);
		if (uv.y > 0.0 || uv.x + uv.y < det)
			return 0;
	} else
		return false;

	distance = Vector3::DotProduct(edge2, qvec) * inv_det;
	uv = uv * inv_det;

	return true;
}

bool LightEffect(Vector3 const &lPos, Vector3 const &dir, float distance)
{
	Scene &scene = Scene::Get();

	for (unsigned int i = 0; i < scene.objects.size(); i++) {
		const Mesh &mesh = scene.objects[i].mesh;
		for (unsigned int t = 0; t < mesh.faces_size; t += 3) {
			float rayDistance;
			Vector2 uv;
			if (IntersectTriangle(lPos, dir, mesh.tVertices + t,
					      rayDistance, uv) &&
			    rayDistance < distance - EPSILON * 10)
				return true;
		}
	}
	return false;
}

void Render::CalculatePixel(RenderData *_data, unsigned int start,
			    unsigned int end)
{
	Scene &scene = Scene::Get();

	for (auto &light : scene.lights)
		light.tPos = camTransform(light.pos);

	for (unsigned int y = start; y < end; y++) {
		for (unsigned int x = 0; x < with; x++) {
			const unsigned int index = y * with + x;

			unsigned int search = 0;
			float distance = std::numeric_limits<float>::max();

			for (unsigned int i = 0; i < THREAD_NUMBER; i++) {
				float &tmp = _data[i].zBuffer[index];
				if (tmp < distance) {
					distance = tmp;
					search = i;
				}
				tmp = std::numeric_limits<float>::max();
			}

			if (distance < minView || distance > maxView)
				continue;

			RenderData &found = _data[search];
			(void)found;

			const Vector3 dir = Vector3(halfWith - (int)x,
						    halfHeight - (int)y, height)
						.Normalized();
			Vector3 pos = dir * distance;

			Vector3 normal = TrisLerp(found.trisData[index].normals,
						  found.tuv[index]);

			const Light &light = scene.lights[0];

			const Vector3 lPos = light.tPos;

			const Vector3 lDiff = pos - lPos;
			const float lDistance = lDiff.Magnitude();
			const Vector3 lDir = lDiff / lDistance;

			//-----------light---------
			// 0-255
			unsigned int color =
			    (1 - Vector3::DotProduct(normal, lDir)) * 127;
			color /= MaxFloat(1, lDistance * lDistance * lDistance);
			color |= 0xFF000000;

			window.SetPixel(x, y, color);

			//-----------shadow---------
			// if (LightEffect(lPos, lDir, lDistance))
			// 	window.SetPixel(x, y, 0xFF000000);
			// else
			// 	window.SetPixel(
			// 	    x, y,
			// 	    normalToColor(
			// 		Vector3::One() *
			// 		-Vector3::DotProduct(normal, lDir)));

			//-----------local_normal---------
			// window.SetPixel(x, y,
			// 		normalToColor(TrisLerp(
			// 		    found.trisData[index].normals,
			// 		    found.tuv[index])));

			//-----------local_position---------
			//window.SetPixel(x, y, normalToColor(pos));

			
		}
	}
}

void Render::View()
{
	Scene &scene = Scene::Get();

	for (unsigned int i = 0; i < scene.objects.size(); i++) {

		const unsigned int faces_size =
		    scene.objects[i].mesh.faces_size;

		float start = 0;
		const float piece = (float)faces_size / (THREAD_NUMBER * 3);

		for (unsigned int j = 0; j < THREAD_NUMBER; j++) {

			data[j].faces_start = RoundToInt(start) * 3;
			data[j].faces_end = RoundToInt(start += piece) * 3;

			threads[j] = std::thread{&Render::RenderObject, this,
						 std::ref(scene.objects[i]),
						 std::ref(data[j])};
		}
	}

	for (unsigned int i = 0; i < THREAD_NUMBER; i++)
		threads[i].join();

	window.Clear(0);

	float start = 0;
	const float piece = (float)height / THREAD_NUMBER;

	for (unsigned int j = 0; j < THREAD_NUMBER; j++) {
		const unsigned int start_pos = RoundToInt(start);
		const unsigned int end_pos = RoundToInt(start += piece);
		threads[j] = std::thread{&Render::CalculatePixel, this, data,
					 start_pos, end_pos};
	}

	for (unsigned int i = 0; i < THREAD_NUMBER; i++)
		threads[i].join();

	window.UpdateSurface();
}