#include <DirectionalLight.hpp>

DirectionalLight::DirectionalLight() {}

DirectionalLight::~DirectionalLight() {}

Vector3 TransformLight() {
	return Vector3::Zero();
}

static inline bool controlFunctionPoint(Vector2 const &a, Vector2 const &b, Vector2 const &point) {
	const Vector2 edge1 = a - b;
	const Vector2 edge2 = b - point;

	return (edge1.x * edge2.y) - (edge1.y * edge2.x) > 0;
}
/*
void DirectionalLight::CalculateDirectionalLightDepth(Object const &object,
                    unsigned int faces_start,
                    unsigned int faces_end)
{
    const Mesh &mesh = object.mesh;
    for (unsigned int t = faces_start; t < faces_end; t += 3) {

        const Vector3 worldPos[3] = {transform(mesh.vertices[t]),
                         transform(mesh.vertices[t + 1]),
                         transform(mesh.vertices[t + 2])};

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

        const int aMaxY = std::min(tris2d[1].y, (float)height - 1);
        for (int y = std::max(0.0f, tris2d[0].y + 1); y <= aMaxY; y++) {
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

            const float cL = 1.0f / (max - min);
            const int aMaxX = std::min(max, (float)with - 1);
            for (int x = std::max(0.0f, min + 1); x <= aMaxX; x++) {
                const float rZPos =
                    zPosA * (x - min) * cL +
                    zPosB * (1.0f - (x - min) * cL);
                if (rZPos < shadowMap[y * height + x]) {
                    shadowMap[y * height + x] = rZPos;
                }
            }
        }

        const int bMaxY = std::min(tris2d[2].y, (float)height - 1);
        for (int y = std::max(0.0f, tris2d[1].y + 1); y <= bMaxY; y++) {
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

            const float cL = 1.0f / (max - min);
            const int bMaxX = std::min(max, (float)with - 1);
            for (int x = std::max(0.0f, min + 1); x <= bMaxX; x++) {
                const float rZPos =
                    zPosA * (x - min) * cL +
                    zPosB * (1.0f - (x - min) * cL);
                if (rZPos < shadowMap[y * height + x]) {

                    shadowMap[y * height + x] = rZPos;
                }
            }
        }
    }
}*/