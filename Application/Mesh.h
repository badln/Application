#pragma once
#include "main.h"
#include "Vertex.h"
#include "Entity.h"
#define KTY_VERTEX_SIZE sizeof(Vertex)

class Mesh {
protected:
	unsigned int _vArray, _vBuffer, _elementBuffer;
	bool assigned;
public:

	glm::mat4 ModelMatrix = glm::mat4(1);
	Transform uniqueTransform;

	static Vector3 computeNormal(Vector3 a, Vector3 b, Vector3 c);
	static std::vector<Vector3> CalculateNormals(std::vector<Vector3> positions, std::vector<unsigned int> indices = std::vector<unsigned int>{});
	Mesh(std::vector<Vertex> newVerties, std::vector<unsigned int> newIndices);
	Mesh();
	bool initialised() const { return assigned; }
	unsigned int vArray() const { return _vArray; }
	unsigned int vBuffer() const { return _vBuffer; }
	unsigned int elementBuffer() const { return _elementBuffer; }

	int topology = GL_TRIANGLES;

	void BindVArray() {
		glBindVertexArray(_vArray);
	}
	/// <summary>Create mesh from vertices</summary>
	void SetupMesh()
	{
		if (data.vertices.empty())
			return;

		//In case of mesh reassignment
		if (assigned)
		{
			glDeleteVertexArrays(1, &_vArray);
			glDeleteBuffers(1, &_vBuffer);
			glDeleteBuffers(1, &_elementBuffer);
		}

		glGenVertexArrays(1, &_vArray);
		glGenBuffers(1, &_vBuffer);
		glGenBuffers(1, &_elementBuffer);
		glBindVertexArray(_vArray);	

		glBindBuffer(GL_ARRAY_BUFFER, _vBuffer);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * KTY_VERTEX_SIZE, &data.vertices[0], GL_STATIC_DRAW);
		if (data.indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), &data.indices[0], GL_STATIC_DRAW);
		}
		//positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, KTY_VERTEX_SIZE, (void*)offsetof(Vertex, Vertex::Position));
		//normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, KTY_VERTEX_SIZE, (void*)offsetof(Vertex, Vertex::Normal));
		//colours
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, KTY_VERTEX_SIZE, (void*)offsetof(Vertex, Vertex::Colour));
		//texcoords
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, KTY_VERTEX_SIZE, (void*)offsetof(Vertex, Vertex::TexCoord)); //Vector2's

		assigned = true;
		glBindVertexArray(0);
	}
	struct {
		std::vector<Vertex>	      vertices;
		std::vector<unsigned int> indices;
		//std::vector<Texture>	  textures;
	} data;
};

class MeshGenerator {
private:
	static void GenFaceIndex(std::vector<unsigned int>* indices, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4);
public:

	static Mesh ToMesh(int topology, std::vector<Vector3> Positions, std::vector<Vector2> UVs, std::vector<Vector3> Normals, std::vector<unsigned int> indices = std::vector<unsigned int>{});

	static Mesh Cube(float size = 1);
	static Mesh Quad(float width = 1, float height = 1);
	static Mesh Plane(unsigned int xSegments, unsigned int ySegments); 
	static Mesh Circle(unsigned int sides = 12);
	static Mesh CubeSphere(int subdivisions = 4);
	static Mesh UVSphere(int slices = 32, int stacks = 16);
};
