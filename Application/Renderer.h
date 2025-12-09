#ifndef KTY_RENDERE
#define KTY_RENDERER

#include "main.h"
#include "Entity.h"
#include "Vertex.h"
#include "Mesh.h"
#include "DefaultShaders.h"
#include "Material.h"
#include "Framebuffer.h"

enum DrawMode {
	Backface,
	Frontface,
	Wireframe, 
	NoCull
};

using namespace glm;

class MeshRenderer : public EntityComponent {
public:
	Mesh mesh;
	Material material = Material(); 
	DrawMode drawMode = DrawMode::Backface;
	bool autoDraw = true;

	void Start() override {
		mesh.SetupMesh();
	}
	void RenderFunc() override {
		if (autoDraw)
			Draw(material.shader);
	}
	void SetupMesh() {
		mesh.SetupMesh();
	}
	void SendMaterialTexture(Shader* s, Texture* t, const char* uniform) {
		glActiveTexture(GL_TEXTURE0 + t->ID());
		t->BindTexture();
		glGenerateMipmap(GL_TEXTURE_2D);
		s->SetVar<Texture*>(t, uniform);

	}
	void SendMaterialData(Shader* s) {
		
		material.diffuse.CreateTexture  (Vector2(1, 1), GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4, Colour(1.0f));
		material.normal.CreateTexture   (Vector2(1, 1), GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4, Colour(1.0f));
		material.emissive.CreateTexture (Vector2(1, 1), GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4, Colour(0.0f));
		material.specular.CreateTexture (Vector2(1, 1), GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4, Colour(0.5f));

		s->SetVar<Colour>(material.diffuseColour,  "mat.diffuse" );
		s->SetVar<Colour>(material.specularColour, "mat.specular");
		s->SetVar<Colour>(material.emissiveColour, "mat.emissive");
		s->SetVar<float> (material.shininess,      "mat.shininess");
		s->SetVar<float> (material.alpha,          "mat.alpha");

		SendMaterialTexture(s, &material.diffuse,  "mat.diffuse_tex" );
		SendMaterialTexture(s, &material.specular, "mat.specular_tex");
		SendMaterialTexture(s, &material.emissive, "mat.emissive_tex");
		SendMaterialTexture(s, &material.normal,   "mat.normal_tex"  );
	}

	glm::mat4 localWorldMatrix(Entity* e) {
		if (e->Parent() != nullptr)
			return (e->transform.WorldMatrix(&e->Parent()->transform));
		else
			return (e->transform.WorldMatrix());

	}

	void Draw(Shader* s) {
		if (mesh.initialised() != 1)
			return;
		s->Use();

		s->SetVar<mat4>(localWorldMatrix(entity), "mat4_model");
		s->SetVar<mat4>(Camera::activeCamera->perspectiveMatrix, "mat4_perspective");
		s->SetVar<mat4>(Camera::activeCamera->viewMatrix, "mat4_camera");

		s->SetVar<Vector3>(Camera::activeCamera->entity->transform.position, "camera_position");
		s->SetVar<Vector2>(KTYGlobal::renderSize, "renderRes");

		SendMaterialData(s);

		mesh.BindVArray();
		switch (drawMode) {
		case (Wireframe):
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			break;
		case Backface:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Frontface:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case NoCull:
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;


		}
		if (mesh.data.indices.size() == 0) {
			glDrawArrays(mesh.topology, 0, mesh.data.vertices.size());
		}
		else
			glDrawElements(mesh.topology, mesh.data.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};


#endif