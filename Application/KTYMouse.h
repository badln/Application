#pragma once
#include "main.h"
#include "KTYMath.h"
#include "global.h"

#define mouse_left 0x01
#define mouse_right 0x02
#define mouse_middle 0x04
#define mouse_x1 0x05
#define mouse_x2 0x06

#define pressed 0
#define depressed 1
#define pressed_this_frame 2
#define depressed_this_frame 3

namespace Input{

	enum CursorState {
		Visible,
		Hidden,
		Disabled,
		Captured,
		Unavailable
	};
	class Mouse {
	private:

		static RAWINPUTDEVICE mouse_rid;

		static bool _deltaFlag;
		static bool _statechanged;

		static POINT cPoint;
		static POINT newPoint;

		static Vector2 cPointDelta;
		static Vector2 cPointDeltaLF;
		static Vector2 screenCoordinates;

		static CursorState _cursorState;
		static void ScreenCoordinates() {
			GetCursorPos(&cPoint);
			screenCoordinates = Vector2(cPoint.x, cPoint.y);
		}
		static void _setstate() {
			if (!_statechanged)
				return; 
			switch (_cursorState) {
			case (CursorState::Visible):
				glfwSetInputMode(KTYGlobal::MainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case (CursorState::Hidden):
				glfwSetInputMode(KTYGlobal::MainWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case (CursorState::Captured):
				glfwSetInputMode(KTYGlobal::MainWindow(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
				break;
			case (CursorState::Disabled):
				glfwSetInputMode(KTYGlobal::MainWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case (CursorState::Unavailable):
				glfwSetInputMode(KTYGlobal::MainWindow(), GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);
				break;
			}
		}
		static bool button_press(int code) {
			int ButtonState = GetKeyState(code);
			if (ButtonState == -127 || ButtonState == -128)
				return true;
			return false;
		}
		static inline int buttons[6]{ 0 };
		static inline int old_buttons[6]{ 0 };

		static void delta(GLFWwindow* window, double xposIn, double yposIn) {

			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);
#
			_deltaFlag = true;

			float x =0, y =0;

			if (_cursorState == CursorState::Disabled)
			{
				if (!_statechanged) {
					x = xpos - KTYGlobal::windowSize.x / 2;
					y = ypos - KTYGlobal::windowSize.y / 2;
				}
			}
			else {
				x = xpos - (double)cPointDeltaLF.x;
				y = ypos - (double)cPointDeltaLF.y;
			}

			x *= sensitivity;
			y *= sensitivity;

			cPointDelta = Vector2(x, y);
			cPointDeltaLF = Vector2(xpos, ypos);
		}

	public:

		static float sensitivity;

		static bool enabled;
		static bool Button(int buttonState, int buttonCode, bool IgnoreDefocus = false) {

			if ((!KTYGlobal::WindowFocused() && !IgnoreDefocus) && enabled)
				return false;

			buttons[buttonCode] = GetKeyState(buttonCode);
			switch(buttonState) { 
			case pressed:
				if (button_press(buttonCode))
					return true;
				break;
			case depressed:
				if (!button_press(buttonCode))
					return true;
				break;
			case pressed_this_frame:
				if (button_press(buttonCode) && buttons[buttonCode] != old_buttons[buttonCode])
					return true;
				break;
			case depressed_this_frame:
				if (!button_press(buttonCode) && buttons[buttonCode] != old_buttons[buttonCode])
					return true;
				break;
			}
			return false;
		}

		static bool IsMouseMoving() { return _deltaFlag; }

		static void SetMouseCallback() {
			glfwSetCursorPosCallback(KTYGlobal::MainWindow(), delta);
		}

		// i hate this
		static void MousePreUpdate() {
			_statechanged = false;
			_deltaFlag = false;
			if (newPoint.x != 0 && newPoint.y != 0)
				SetCursorPos(newPoint.x, newPoint.y);
			newPoint.x = 0;
			newPoint.y = 0;
		}

		static void MouseUpdate() {
			ScreenCoordinates();
			if (!_deltaFlag) {
				cPointDelta = Vector2::zero();
			}
			if (_cursorState == CursorState::Disabled)
				CenterCursor(KTYGlobal::MainWindow());
			_setstate();
		}
		static Vector2 CursorDelta() { return cPointDelta; }

		static CursorState cursorState() { return _cursorState; }
		static POINT cursorPoint() { return cPoint; }

		static bool CursorInWindow(GLFWwindow* window) {
			int x, y;
			glfwGetWindowPos(window, &x, &y);
			Vector2 point = GetCursorPosScreen();
			if (point.x < x + KTYGlobal::windowSize.x && point.y < y + KTYGlobal::windowSize.y &&
				point.x > x && point.y > y)
				return true;
			return false; 
		}

		static Vector2 GetCursorPosWindowRaw(GLFWwindow* window) {
			Vector2 point = screenCoordinates;
			int x, y;
			glfwGetWindowPos(window, &x, &y);
			point -= Vector2(x, y);
			return point;
		}

		static Vector2 GetCursorPosWindow(GLFWwindow* window) {
			Vector2 point = screenCoordinates;
			int x, y;
			glfwGetWindowPos(window, &x, &y);
			point = Vector2(
				Math::clamp(point.x, (double)x + point.x, x + KTYGlobal::windowSize.x),
				Math::clamp(point.y, (double)y + point.y, y + KTYGlobal::windowSize.y)
			);
			return point;
		}

		static Vector2 GetCursorPosScreen() {
			return screenCoordinates;
		}
		static Vector2 CenterCursor(GLFWwindow* window) {
			return SetCursorPosWindow(window, KTYGlobal::windowSize / 2);
		}

		static Vector2 SetCursorPosWindow(GLFWwindow* window, Vector2 point) {
			if (!KTYGlobal::WindowFocused())
				return Vector2::zero();
			int x, y;
			glfwGetWindowPos(window, &x, &y);
			//Point can't be set out of the bounds of the window. min val is x (window pos top left) and max is x + windowsize.x.
			point = Vector2(
				Math::clamp(point.x, (double)x - point.x, x + KTYGlobal::windowSize.x),
				Math::clamp(point.y, (double)y - point.y, y + KTYGlobal::windowSize.y)
			);
			return SetCursorPosScreen(point + Vector2(x, y));
		}
		static Vector2 SetCursorPosScreen(Vector2 point) {
			if (!KTYGlobal::WindowFocused())
				return Vector2::zero();
			newPoint.x = point.x;
			newPoint.y = point.y;
			return point;
		}

		static void SetCursorState(CursorState newState) {
			_statechanged = true;
			_cursorState = newState;
		}
		static void SwapButtonBuffer(GLFWwindow* window) {
			for (int i = 0; i < std::size(buttons); i++) {
				old_buttons[i] = buttons[i];
			}

		}
	};
}