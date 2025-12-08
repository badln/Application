#include "Mesh.h"


Mesh MeshGenerator::Cube(float size) {
    std::vector<Vector3> Positions = std::vector<Vector3>{
            Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(0.5f,  0.5f, -0.5f),
            Vector3(0.5f, -0.5f, -0.5f),
            Vector3(0.5f,  0.5f, -0.5f),
            Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(-0.5f,  0.5f, -0.5f),

            Vector3(-0.5f, -0.5f,  0.5f),
            Vector3(0.5f, -0.5f,  0.5f),
            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(-0.5f,  0.5f,  0.5f),
            Vector3(-0.5f, -0.5f,  0.5f),

            Vector3(-0.5f,  0.5f,  0.5f),
            Vector3(-0.5f,  0.5f, -0.5f),
            Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(-0.5f, -0.5f,  0.5f),
            Vector3(-0.5f,  0.5f,  0.5f),

            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(0.5f, -0.5f, -0.5f),
            Vector3(0.5f,  0.5f, -0.5f),
            Vector3(0.5f, -0.5f, -0.5f),
            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(0.5f, -0.5f,  0.5f),

            Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(0.5f, -0.5f, -0.5f),
            Vector3(0.5f, -0.5f,  0.5f),
            Vector3(0.5f, -0.5f,  0.5f),
            Vector3(-0.5f, -0.5f,  0.5f),
            Vector3(-0.5f, -0.5f, -0.5f),

            Vector3(-0.5f,  0.5f, -0.5f),
            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(0.5f,  0.5f, -0.5f),
            Vector3(0.5f,  0.5f,  0.5f),
            Vector3(-0.5f,  0.5f, -0.5f),
            Vector3(-0.5f,  0.5f,  0.5f),
    };
    std::vector<Vector2> UV = std::vector<Vector2>{
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 1.0f),

        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),

        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 0.0f),

        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 0.0f),

        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 1.0f),

        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
    };
    std::vector<Vector3> Normals = Mesh::CalculateNormals(Positions);
    return ToMesh(GL_TRIANGLES, Positions, UV, Normals);
}