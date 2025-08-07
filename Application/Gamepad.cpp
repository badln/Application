#include "Gamepad.h"
float clamp(float num, float min, float max)
{
	if (num > max)
		num = max;
	if (num < min)
		num = min;
	return num;
}
float Gamepad::CalculateDeadZone(float num) {
	if (num < deadZone && num > 0 || num > -deadZone && num < 0)
		num = 0;
	return num;
}
float Gamepad::LEFT_STICK_X()
{
	return CalculateDeadZone(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
}
float Gamepad::LEFT_STICK_Y()
{
	return CalculateDeadZone(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
}
float Gamepad::RIGHT_STICK_X()
{
	return CalculateDeadZone(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
}
float Gamepad::RIGHT_STICK_Y()
{
	return CalculateDeadZone(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
}
vec2 Gamepad::LEFT_STICK(double ft)
{
	vec2 ret = vec2(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_X], padState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
	ret.x = CalculateDeadZone(ret.x);
	ret.y = CalculateDeadZone(ret.y);

	//NaN prevention (needed)
	if (ret.x != ret.x)
		ret.x = 0;
	if (ret.y != ret.y)
		ret.y = 0;
	
	ret = vec2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));
	LS_X_ADDITIVE += ret.x * Sensitivity * ft;
	LS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}
vec2 Gamepad::RIGHT_STICK(double ft)
{
	vec2 ret = vec2(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
	ret.x = CalculateDeadZone(ret.x);
	ret.y = CalculateDeadZone(ret.y);
	
	//NaN prevention (needed)
	if (ret.x != ret.x)
		ret.x = 0;
	if (ret.y != ret.y)
		ret.y = 0;
	
	ret = vec2(clamp(ret.x, -1, 1), clamp(ret.y, -1, 1));
	RS_X_ADDITIVE += ret.x * Sensitivity * ft;
	RS_Y_ADDITIVE += ret.y * Sensitivity * ft;
	return ret;
}
