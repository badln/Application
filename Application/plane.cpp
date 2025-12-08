#include "Mesh.h"

Mesh MeshGenerator::Plane(unsigned int xSegments, unsigned int ySegments) {
	std::vector<Vector3> Positions;
	std::vector<Vector2> UV;
	std::vector<Vector3> Normals;
	std::vector<unsigned int> indices;

	float deltaX = 1.0f / xSegments;
	float deltaY = 1.0f / ySegments;

	for (int y = 0; y <= ySegments; ++y) {
		for (int x = 0; x <= xSegments; ++x) {
            Positions.push_back(Vector3(deltaX * x * 2.0f - 1.0f, deltaY * y * 2.0f - 1.0f, 0.0f));
			UV.push_back(Vector2(deltaY * x, 1.0f - y * deltaY));
			Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		}
	}

    for (int y = 0; y < ySegments; ++y)
    {
        if ((y % 2 != 0))
        {
            for (int x = 0; x <= xSegments; ++x)
            {
                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y * (xSegments + 1) + x);
            }
        }
        else
        {
            for (int x = xSegments; x >= 0; --x)
            {
                indices.push_back(y * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x);
            }
        }
    }
	return ToMesh(GL_TRIANGLE_STRIP, Positions, UV, Normals, indices);
}