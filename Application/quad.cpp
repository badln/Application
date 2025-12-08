#include "Mesh.h"

Mesh MeshGenerator::Quad(float width, float height) {

    std::vector<Vector3> Positions = std::vector<Vector3> {
        Vector3(- width,  height, 0.0f),
        Vector3(- width, -height, 0.0f),
        Vector3(  width,  height, 0.0f),
        Vector3(  width, -height, 0.0f),
    };
    std::vector<Vector2> UV = std::vector<Vector2>{
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 0.0f),
    };
    std::vector<unsigned int> indices = std::vector<unsigned int>{ 
        0, 1, 2,
        2, 1, 3
    };
    std::vector<Vector3> Normals = Mesh::CalculateNormals(Positions, indices);
    return ToMesh(GL_TRIANGLES, Positions, UV, Normals, indices);
}