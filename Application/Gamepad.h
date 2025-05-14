#pragma once
#include "main.h"

class Gamepad {
	float CalculateDeadZone(float num);
	float LEFT_STICK_Y();
	float LEFT_STICK_X();
	float RIGHT_STICK_Y();
	float RIGHT_STICK_X();
public:
	GLuint thisPad = GLFW_JOYSTICK_1;

	bool padConnected = false;
	std::string padName;
	GLFWgamepadstate padState;
	float deadZone = 0.1f;
	float LS_X_ADDITIVE, LS_Y_ADDITIVE;
	float RS_X_ADDITIVE, RS_Y_ADDITIVE;
	float Sensitivity = 15;

	vec2 LEFT_STICK();
	vec2 RIGHT_STICK();

};