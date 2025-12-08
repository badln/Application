#ifndef KTY_SETTINGS_H
#define KTY_SETTINGS_H

#include "main.h"

#define KTY_FULLSCREEN 0
#define KTY_WINDOWED   1
#define KTY_BORDERLESS 2

class KTYGlobal {
private:
	static const GLFWvidmode* _monitorInfo;
	static GLFWwindow* _mainWindow;
	static int _glMaxTextures;
	static int _frame;
	static bool _focus; 
public:
	static const int MaxLights = 32;
	static GLFWwindow* MainWindow() { return _mainWindow; }
	static GLFWwindow* SetMainWindow(GLFWwindow* window) { return _mainWindow = window; }
	static const GLFWvidmode* MonitorInfo() { return _monitorInfo; }
	static void UpdateImportantStuff(GLFWwindow* window) {
		_focus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		renderSize = Vector2(Math::clamp(renderSize.x, (double)1, windowSize.x * 2), Math::clamp(renderSize.y, (double)1, windowSize.y * 2));
	}
	static Vector2 MonitorSize() { return Vector2(_monitorInfo->width, _monitorInfo->height); }
	static bool WindowFocused() { return _focus; }
	static int GLMaxTextures() { return _glMaxTextures; }
	static int CurrentFrame() { return _frame; }
	static void increment_frame() {
		_frame++;
	}
	static void gl_max_t(int x) {
		_glMaxTextures = x;
	}
	
	static Vector2 windowSize;
	static Vector2 renderSize;
	inline static const int currentWindowType = KTY_WINDOWED;


	static	void UpdateMonitorInfo(GLFWmonitor* monitor = glfwGetPrimaryMonitor()) {
		_monitorInfo = glfwGetVideoMode(monitor);
	}

	struct EngineInfo {
		inline static const std::string name = "KittyEngine";
		inline static const std::string ver = "1.0.0";
		inline static const std::string buildDate = "06/11/25";
	};
	static void LogEngineInfo()
	{
		std::cout << EngineInfo::name << " Version " << EngineInfo::ver << " Built " << EngineInfo::buildDate << std::endl;
	}

};
#endif