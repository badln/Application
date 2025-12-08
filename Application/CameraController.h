#pragma once
#include "Entity.h";
#include "KTYKeyboard.h"
#include "KTYMouse.h"
#include "Camera.h"
#include "ScreenshotPNG.h"

using namespace Input;

class CameraController : public EntityComponent {
public:
	bool flag = false;
	Vector2 pos;

	Entity* camera;
	Entity* camera2;

	float mult = 3;

	void Update() override {

		if (Keyboard::Key(pressed_this_frame, key_f12))
			Screenshot::TakeScreenshot();

		if (Mouse::Button(pressed_this_frame, mouse_right)) {
			pos = Mouse::GetCursorPosScreen();
			Mouse::SetCursorState(CursorState::Disabled);
		}

		if (Keyboard::Key(pressed, key_up)) {
			Time::timescale += 1 * Time::deltaTime();
		}
		if (Keyboard::Key(pressed, key_down)) {
			Time::timescale -= 1 * Time::deltaTime();
		}

		(Time::timescale <= 0) ? Time::timescale = 0 : NULL;

		
		if (Mouse::Button(pressed, mouse_right)) {
			Camera::mainCamera->Look(Mouse::CursorDelta());
		}

		if (Mouse::Button(depressed_this_frame, mouse_right)) {
			Mouse::SetCursorPosScreen(pos);
			Mouse::SetCursorState(CursorState::Visible);
		}

		if (Keyboard::Key(pressed, key_W))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.forward() * mult;

		if (Keyboard::Key(pressed, key_S))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.back() * mult;



		if (Keyboard::Key(pressed, key_E))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.up() * mult;

		if (Keyboard::Key(pressed, key_Q))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.down() * mult;



		if (Keyboard::Key(pressed, key_A))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.left() * mult;

		if (Keyboard::Key(pressed, key_D))
			Camera::mainCamera->entity->transform.localPosition += Time::deltaTime() * Camera::mainCamera->entity->transform.right() * mult;

		if (Keyboard::Key(pressed_this_frame, key_R));
			//(Camera::mainCamera == &camera->GetComponent<Camera>()) ? camera2->GetComponent<Camera>().SetMainCamera() : camera->GetComponent<Camera>().SetMainCamera();
	}
};