#include "Mesh.h"

Mesh MeshGenerator::UVSphere(int slices, int stacks) {
    std::vector<Vector3> Positions;
    std::vector<Vector2> UV;
    std::vector<Vector3> Normals;
    std::vector<unsigned int> indices;
    for (unsigned int y = 0; y <= stacks; ++y)
    {
        for (unsigned int x = 0; x <= slices; ++x)
        {
            float xSegment = (float)x / (float)slices;
            float ySegment = (float)y / (float)stacks;

            Vector3 pos = Vector3(
                std::cos(xSegment * Math::tau) * std::sin(ySegment * Math::pi),
                std::cos(ySegment * Math::pi),
                std::sin(xSegment * Math::tau) * std::sin(ySegment * Math::pi)
            );

            Positions.push_back(pos);
            UV.push_back(Vector2(xSegment, ySegment));
            Normals.push_back(-pos.Normalized());
        }
    }

    for (int y = 0; y < stacks; ++y)
    {
        for (int x = 0; x < slices; ++x)
        {
            indices.push_back((y + 1) * (slices + 1) + x);
            indices.push_back( y *      (slices + 1) + x);
            indices.push_back( y *      (slices + 1) + x + 1);

            indices.push_back((y + 1) * (slices + 1) + x);
            indices.push_back( y *      (slices + 1) + x + 1);
            indices.push_back((y + 1) * (slices + 1) + x + 1);
        }
    }
    return ToMesh(GL_TRIANGLES, Positions, UV, Normals, indices);

}