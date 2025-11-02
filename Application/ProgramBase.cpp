#include "ProgramBase.h"
#include "KTYEngine.h"

int Program::Begin(std::string windowName, int winType) {

	PreInit();
	EngineInfo.windowSize = windowSize.glm();

	Time::Init();
	InitLogger();

	GetSystemInfo(&DeviceInfo::SystemInfo);

	DeviceInfo::MemoryStatus.dwLength = sizeof(DeviceInfo::MemoryStatus);
	GlobalMemoryStatusEx(&DeviceInfo::MemoryStatus);

	GLFWLog.CLog("Attempting to create GLFW Context '" + windowName + "'");
	GLFWLog.CLog("Window Type: " + std::to_string(winType));
	GLFWLog.CLog("Window Size: " + std::to_string((int)windowSize.x()) + "x" + std::to_string((int)windowSize.y()));
	GLFWLog.CLog("Initialising GLFW");

	glfwInit();
	if (!glfwInit())
	{
		GLFWLog.Error("GLFW Failed to initialise for an unknown reason. Exited with code -1.");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, vidMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, vidMode->refreshRate);


	monitor = NULL;
	switch (winType)
	{
	case 0:
		monitor = glfwGetPrimaryMonitor();
		break;
	case 1:
		glfwWindowHint(GLFW_DECORATED, 1);
		break;
	case 2:
		glfwWindowHint(GLFW_DECORATED, 0);
		break;
	}

	thisWindow = glfwCreateWindow(windowSize.x(), windowSize.y(), windowName.c_str(), monitor, NULL);
	//glfwSetWIndow sets window position from top left corner of window. 
	//Given the screen is 1080p and the windiw is 1280x800, x padding is 320.
	//320 + 1280 + 320 = 1920, 320 is padding. 
	glfwSetWindowPos(thisWindow, (vidMode->width - windowSize.x()) / 2 /* x padding */, (vidMode->height - windowSize.y()) / 2 /* y padding */);
	if (thisWindow == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(thisWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  //GLAD must be initialised after a window is created.
	{
		GLFWLog.Error("GLAD Init failed! Exited with code -1");
		return -1;
	}

	std::string displayAdaptor(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	std::string glVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

	GLFWLog.Empty();
	GLFWLog.CLog("MONITOR VIDEO MODE");
	GLFWLog.CLog("Red bits: " + std::to_string(vidMode->redBits));
	GLFWLog.CLog("Green bits: " + std::to_string(vidMode->blueBits));
	GLFWLog.CLog("Blue bits: " + std::to_string(vidMode->greenBits));

	std::string res = std::to_string(vidMode->width) + (std::string)"x" + std::to_string(vidMode->height) + (std::string)" @ " + std::to_string(vidMode->refreshRate) + "Hz";

	GLFWLog.CLog("Monitor Resolution: " + res);
	GLFWLog.Empty();

	GLFWLog.CLog("DEVICE INFO");
	GLFWLog.CLog("Display Adaptor: " + displayAdaptor);
	GLFWLog.CLog("OpenGL Version: " + glVersion);
	GLFWLog.CLog("CPU: " + DeviceInfo::CPUName());
	GLFWLog.CLog("Memory: " + std::to_string((int)((float)DeviceInfo::MemoryStatus.ullAvailPhys / 1024 / 1024))
		+ "MB Free of " + std::to_string((int)((float)DeviceInfo::MemoryStatus.ullTotalPhys / 1024 / 1024)) + "MB total. ("
		+ std::to_string((int)DeviceInfo::MemoryStatus.dwMemoryLoad) + "% of " + std::to_string((int)((float)DeviceInfo::MemoryStatus.ullTotalPhys / 1024 / 1024 / 1024)) + "GB RAM used.)");
	GLFWLog.CLog(std::to_string(DeviceInfo::SystemInfo.dwNumberOfProcessors) + " CPU threads available");

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//SetWindowIcon("Images/IMG_4766.png", window);

	GLFWLog.CLog("Initialising Object System");
	Object.Init();
	//Object.RegisterComponent<Transform>();

	GLFWLog.CLog("GLFW Init Time: " + std::to_string(Time::SinceStartup()));
	MainLoop();
}