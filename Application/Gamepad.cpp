#include "Gamepad.h"

// TODO : Separate this from glfw.

float clamp(float num, float min, float max)
{
	if (num > max)
		num = max;
	if (num < min)
		num = min;
	return num;
}
float Gamepad::length(vec2 v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}
float Gamepad::angle(vec2 v1, vec2 v2)
{
	float dot = v1.x * v2.x + v1.y * v2.y;
	float det = v1.x * v2.y - v1.y * v2.x;
	return atan2(det, dot);
}
vec2 Gamepad::rotate(vec2 v, float angleInRads) 
{
		float s = sin(angleInRads);
		float c = cos(angleInRads);
		vec2 r;
		r.x = v.x * c - v.y * s;
		r.y = v.x * s + v.y * c;
		return r;
}
vec2 Gamepad::CalculateDeadZone(vec2 v) {
	float len = length(v);
	if (len < deadZone)
		return vec2(0);
	return v;
}
vec2 Gamepad::LEFT_STICK(double ft)
{
	vec2 ret = vec2(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_X], padState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
	ret = CalculateDeadZone(ret);
	if (ret.x != ret.x)
		ret.x = 0;

	if (ret.y != ret.y)
		ret.y = 0;
	ret = vec2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));

	if (length(ret) > 1)
		ret = rotate(vec2(1, 0), -angle(ret, vec2(1, 0)));

	LS_X_ADDITIVE += ret.x * Sensitivity * ft;
	LS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}
vec2 Gamepad::RIGHT_STICK(double ft)
{
	vec2 ret = vec2(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
	ret = CalculateDeadZone(ret);
	if (ret.x != ret.x)
		ret.x = 0;

	if (ret.y != ret.y)
		ret.y = 0;
	ret = vec2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));

	if (length(ret) > 1) 
		ret = rotate(vec2(1, 0), -angle(ret, vec2(1, 0)));

	RS_X_ADDITIVE += ret.x * Sensitivity * ft;
	RS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}