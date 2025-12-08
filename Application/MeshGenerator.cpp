#include "Mesh.h"
void MeshGenerator::GenFaceIndex(std::vector<unsigned int>* indices, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3) {
    std::vector<unsigned int> face{
        p0, p1, p2,
        p2, p1, p3
    };
    for (int i = 0; i < face.size(); i++) {
        indices->push_back(face[i]);
    }
}

Mesh MeshGenerator::ToMesh(int topology, std::vector<Vector3> Positions, std::vector<Vector2> UVs, std::vector<Vector3> Normals, std::vector<unsigned int> indices) {
    std::vector<Vertex> vertices;
    for (int i = 0; i < Positions.size(); i++) {
        Vertex v;
        v.Position = Positions[i].glm();
        v.TexCoord = UVs[i].glm();
        v.Normal = Normals[i].glm();
        v.Colour = Vector3(1).glm();
        vertices.push_back(v);
    }
    Mesh mesh(vertices, indices);
    mesh.topology = topology;
    return mesh;
}
Vector3 Mesh::computeNormal(Vector3 a, Vector3 b, Vector3 c) {
    return Vector3(glm::cross((c - a).glm(), (b - a).glm())).Normalized();
}
std::vector<Vector3> Mesh::CalculateNormals(std::vector<Vector3> positions, std::vector<unsigned int> indices) {
    
    std::vector<Vector3> normals;
    for (int i = 0; i < positions.size(); i++) {
        normals.push_back(Vector3(0));
    }

    //works given triangles
    if (indices.size() == 0) {
        for (int i = 0; i < positions.size(); i += 3) {
            Vector3 a = positions[i];
            Vector3 b = positions[i + 1];
            Vector3 c = positions[i + 2];

            Vector3 normal = Mesh::computeNormal(a, b, c);

            normals[i] = normal;
            normals[i + 1] = normal;
            normals[i + 2] = normal;
        }
    }
    else {
        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            Vector3 a = positions[indices[i]];
            Vector3 b = positions[indices[i + 1]];
            Vector3 c = positions[indices[i + 2]];
            
            Vector3 normal = Mesh::computeNormal(a, b, c);

            normals[indices[i]] = normal;
            normals[indices[i + 1]] = normal;
            normals[indices[i + 2]] = normal;
        }
    }
    return normals;
}

Mesh::Mesh(std::vector<Vertex> newVerties, std::vector<unsigned int> newIndices) {
    data.vertices = newVerties, data.indices = newIndices;
    SetupMesh();
}
Mesh::Mesh() {};