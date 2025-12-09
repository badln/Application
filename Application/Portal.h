#pragma once
#include "Entity.h";
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "Camera.h"

class Portal : public EntityComponent {
public:

	Portal* otherPortal;
	Entity portalCamera;

	void CreatePortal(Vector3 location, Vector3 rotation, Shader* shader, Portal* other) {

		otherPortal = other;

		entity->AddComponent<MeshRenderer>();
		entity->GetComponent<MeshRenderer>().mesh = MeshGenerator::Quad();
		entity->GetComponent<MeshRenderer>().material.shader = shader;
		entity->transform.localScale = Vector3(1, 1.7, 1);
		
		entity->transform.localPosition = location;
		entity->transform.localRotation = rotation;

		portalCamera = Entity::CreateEntity(entity->name + " Camera");
		portalCamera.AddComponent<Camera>();
		portalCamera.AddComponent<MeshRenderer>();
		//portalCamera.GetComponent<MeshRenderer>().mesh = MeshGenerator::Cube();
		//portalCamera.GetComponent<MeshRenderer>().material.shader = &DefaultShaders::Unlit;
		portalCamera.GetComponent<Camera>().framebuffer = new Framebuffer();
		//portalCamera.transform.localScale = Vector3(0.1f);

		entity->GetComponent<MeshRenderer>().material.diffuse = portalCamera.GetComponent<Camera>().framebuffer->colourBuffer;

		portalCamera.SetParent(other->entity);

	}

	float theta;

	void Update() override {

		if (entity->name == "Portal 2 ") {
			entity->transform.localRotation.y = glm::sin(Time::SinceStartup()) * 30;
		}

		Transform* other = &otherPortal->entity->transform;
		Transform* camTransform = &Camera::mainCamera->entity->transform;

		Vector3 positionOffset = entity->transform.localPosition - camTransform->localPosition;
		positionOffset = Vector3(dot(entity->transform.left(), positionOffset), dot(entity->transform.down(), positionOffset), dot(entity->transform.forward(), positionOffset / entity->transform.localScale.y));

		portalCamera.transform.localRotation = camTransform->localRotation - entity->transform.localRotation;

		portalCamera.transform.localRotation.RotateAngleAxis(180, other->up());
		
		portalCamera.transform.localPosition = positionOffset;
		portalCamera.transform.localPosition.y = (camTransform->GlobalPosition().y - entity->transform.localPosition.y) / entity->transform.localScale.y;
		//std::string s = entity->name;
		//Debug::CLog(s + portalCamera.transform.GlobalPosition().str());

	}
};
class PortalController : public EntityComponent {
public:
	Entity Portal1;
	Entity Portal2;

	Shader portalShader;

	void Start() override {
		portalShader.CreateShader("default_vert.glsl", "portal.glsl");
		
		Portal1 = Entity::CreateEntity("Portal 1 ");
		Portal2 = Entity::CreateEntity("Portal 2 ");

		Portal1.AddComponent<Portal>();
		Portal2.AddComponent<Portal>();

		Portal1.GetComponent<Portal>().CreatePortal(Vector3(-3, -1.35, -3), Vector3(0), &portalShader, &Portal2.GetComponent<Portal>());
		Portal2.GetComponent<Portal>().CreatePortal(Vector3(5, -1.35, -3), Vector3(0, 30, 0), &portalShader, &Portal1.GetComponent<Portal>());
	}

};