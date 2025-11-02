#pragma once
#include "main.h"

class Gamepad {
private:
	vec2 CalculateDeadZone(vec2 v);
	float length(vec2 v);
	float angle(vec2 v1, vec2 v2);
	vec2 rotate(vec2 v, float angleInRads);
public:
	GLuint thisPad = GLFW_JOYSTICK_1;

	bool padConnected = false;
	std::string padName;
	GLFWgamepadstate padState;
	float deadZone = 0.3f;
	float LS_X_ADDITIVE = 0, LS_Y_ADDITIVE = 0;
	float RS_X_ADDITIVE = 0, RS_Y_ADDITIVE = 0;
	double Sensitivity = 1500;

	vec2 LEFT_STICK(double ft);
	vec2 RIGHT_STICK(double ft);

};