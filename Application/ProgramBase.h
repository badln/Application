#ifndef GLFW_WINDOW
#define GLFW_WINDOW

#include "oldengine.h"
#include "Logger.h"

#include "EngineMaths.h"
#include <sysinfoapi.h>
#include <sstream>
#include "DeviceInfo.h"
#include "KTYKeyboard.h"
#include "KTYTime.h"
#include "Shader.h"
#include "Entity.h"

#define KTY_PI 3.1415926535

class Program {
private:
	Time::Timer deltaTimer;
public:

	ObjectSystem Object;

	const GLFWvidmode* vidMode;
	GLFWmonitor* monitor;
	GLFWwindow* thisWindow;
	int currentFrame = 0;
	int targetFrameRate = 60;
	int frameRate = 0;
	Vector2 windowSize = Vector2(800,800);
	bool VsyncState = false;
	Keyboard ActiveKeyboard;
	DebugLogger GLFWLog;

	template <typename T> 
	void SetVsyncState(T val) {}

	template <> void SetVsyncState<int>(int val) {
		if (val == 0)
			VsyncState = false;
		else
			VsyncState = true;
		glfwSwapInterval(val);
	}
	template <> void SetVsyncState<bool>(bool val) {
		VsyncState = val;
		if (val) {
			glfwSwapInterval(1);
			return; 
		}
		glfwSwapInterval(0);
	}

	void SetWindowSize(Vector2 size)
	{
		glfwSetWindowSize(thisWindow, size.x(), size.y());
		glfwSetWindowPos(thisWindow, (vidMode->width - EngineInfo.windowSize.x) / 2 /* x padding */, (vidMode->height - EngineInfo.windowSize.y) / 2 /* y padding */);
	}
	void UpdateWindowSize()
	{
		int width, height;
		glfwGetWindowSize(thisWindow, &width, &height);
		if (width == 0 || height == 0)
			return;
		windowSize = Vector2(width, height);
	}

	int InitLogger(bool logEngineInfo = true) {
		if (logEngineInfo)
			EngineInfo.LogEngineInfo();
		GLFWLog.LoggerName = "GLFW";
		GLFWLog.defaultColour = DBG_CYAN;
		GLFWLog.CLog("GLFW Logger initialised.");
		return 0;
	}

	int Begin(std::string windowName = "GLFW", int winType = EngineInfo.currentWindowType);
	int MainLoop() {
		Start();
		while (!glfwWindowShouldClose(thisWindow)) {
			deltaTimer.StartNew();
			UpdateWindowSize();
			glViewport(0, 0, windowSize.x(), windowSize.y());
			glClear(GL_COLOR_BUFFER_BIT);

			Update();
			Object.Update();
			

			glfwSwapBuffers(thisWindow);
			glfwPollEvents();
			currentFrame++;
			ActiveKeyboard.SwapKeyBuffer();
			deltaTimer.Stop();
			Time::SetDeltaTime(deltaTimer.Value());
			frameRate = 1 / Time::deltaTime();
		}
		glfwTerminate();
		return 0;
	}

	virtual void PreInit() {}
	virtual void Update() {}
	virtual void Start() {}
	void Exit() {
		glfwSetWindowShouldClose(thisWindow, 1);
	}

};
#endif