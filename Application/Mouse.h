#include "main.h"

class Mouse {
public:
	float mouseScrollDir = 0;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	float scrollDir;

	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollDir = yOffset;
	}
	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		//mainCamera->SetCameraDir(window, xposIn, yposIn, looking);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};
