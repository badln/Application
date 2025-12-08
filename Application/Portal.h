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
		portalCamera.GetComponent<Camera>().framebuffer = new Framebuffer();

		entity->GetComponent<MeshRenderer>().material.diffuse = portalCamera.GetComponent<Camera>().framebuffer->colourBuffer;

		portalCamera.SetParent(other->entity);

	}

	void Update() override {

		Transform* other = &otherPortal->entity->transform;
		Transform* camTransform = &Camera::mainCamera->entity->transform;

		
		Vector3 positionOffset = entity->transform.localPosition + camTransform->localPosition;
		portalCamera.transform.localRotation = camTransform->localRotation;
		portalCamera.transform.localPosition = positionOffset;
		Debug::CLog(portalCamera.transform.localRotation);
	}
};
class PortalController : public EntityComponent {
public:
	Entity Portal1;
	Entity Portal2;

	Shader portalShader;

	void Start() override {
		portalShader.CreateShader("default_vert.glsl", "portal.glsl");
		
		Portal1 = Entity::CreateEntity("Portal 1");
		Portal2 = Entity::CreateEntity("Portal 2");

		Portal1.AddComponent<Portal>();
		Portal2.AddComponent<Portal>();

		Portal1.GetComponent<Portal>().CreatePortal(Vector3(-3, 1, -3), Vector3(0), &portalShader, &Portal2.GetComponent<Portal>());
		Portal2.GetComponent<Portal>().CreatePortal(Vector3(5, 1, -3), Vector3(10, 30, 0), &portalShader, &Portal1.GetComponent<Portal>());
	}

};