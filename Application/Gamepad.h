#pragma once
#include "main.h"

class Gamepad {
private:
	Vector2 CalculateDeadZone(Vector2 v);
	float length(Vector2 v);
	float angle(Vector2 v1, Vector2 v2);
	Vector2 rotate(Vector2 v, float angleInRads);
public:
	GLuint thisPad = GLFW_JOYSTICK_1;

	bool padConnected = false;
	std::string padName;
	GLFWgamepadstate padState;
	float deadZone = 0.3f;
	float LS_X_ADDITIVE = 0, LS_Y_ADDITIVE = 0;
	float RS_X_ADDITIVE = 0, RS_Y_ADDITIVE = 0;
	double Sensitivity = 1500;

	Vector2 LEFT_STICK(double ft);
	Vector2 RIGHT_STICK(double ft);

};