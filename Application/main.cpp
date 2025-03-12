#define STB_IMAGE_IMPLEMENTATION

//Some specific OpenGL stuff
#include "stb_image.h"
#include "GLFW\Include\glad\glad.h"
#include "GLFW\Include\glfw3.h"

//general stuff
#include <iostream>
#include "shader.h"
#include "engine.h";
#include "transform.h"
#include <windows.h>
#include "EngineResources/Console.h"

//OpenGL Mathematics
#include "GLFW/Include/glm/glm.hpp"
#include "GLFW/Include/glm/gtc/matrix_transform.hpp"
#include "GLFW/Include/glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

WindowConsole Console;

vec2 windowSize(650, 650);

float vertTruncAmount = 12;
bool truncVerts = false;

bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, aPressed, inWireframe = false;
bool ambientRotation = true;
bool LookAtMouse = true;
int ambientRotationMultiplier = 50;

float desiredFramerate = 60;
float desiredFrametime = 1 / desiredFramerate;
double frameTime, endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;
float AddRotation, xAngle, yAngle = 0;
float maxVal{ 1.0f };
float vertexData[] = {

	//----------------------------------------------//
	// Vertex Position | Vertex Colour  |  TexCoords
	//----------------------------------------------//
	 0.5f,  0.5f,-0.5f, 1.0f, 0.0f, 0.0f,  maxVal,  maxVal,
	 0.5f, -0.5f,-0.5f, 1.0f, 1.0f, 0.0f,  maxVal,  0.0f,
	-0.5f, -0.5f,-0.5f, 0.0f, 1.0f, 0.0f,  0.0f,    0.0f,
	-0.5f,  0.5f,-0.5f, 0.0f, 0.0f, 1.0f,  0.0f,    maxVal,
	-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  maxVal,  maxVal,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  maxVal,  0.0f,
	-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  0.0f,    0.0f,
	 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  maxVal,  0.0f,
	 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  0.0f,    0.0f,
	 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  0.0f,    maxVal,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  0.0f,    maxVal,
	 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  maxVal,    maxVal,
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  maxVal,  maxVal,
	 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  maxVal,  0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f,    0.0f,
	-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f,    maxVal,

};
unsigned int indicies[] = {

	//-----------------------------------------//
	// Defines which verts apply to what triangle
	//-----------------------------------------//
	0,  1,  3, // first triangle
	1,  2,  3, // second triangle
	2,  3,  4,
	2,  4,  5,
	0,  3,  7,
	3,  7,  6,
	1,  0,  8,
	0,  8,  9,
	1,  2,  10,
	1,  10, 11,
	12, 13, 15,
	13, 14, 15,

};

float clamp(float val, float min, float max)
{
	if (val < min)
	{
		val = min;
	}
	if (val > max)
	{
		val = max;
	}
	return val;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

string windowName = "3D Renderer (Prototype)";

//-------------//
// Inputs go here
//-------------//

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
	{
		aPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && aPressed == false)
	{
		//------------------------------------//
		// Toggles wireframe rendering on or off
		//------------------------------------//
		aPressed = true;
		if (inWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			inWireframe = !inWireframe;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			inWireframe = !inWireframe;
		}
	}if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		if (spacePressed)
		{
			spacePressed = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!spacePressed)
		{
			spacePressed = true;
			ambientRotation = !ambientRotation;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		upArrowPressed = true;
		downArrowPressed = false;
	}
	else
	{
		upArrowPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		upArrowPressed = false;
		downArrowPressed = true;
	}
	else
	{
		downArrowPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		leftArrowPressed = true;
		rightArrowPressed = false;
	}
	else
	{
		leftArrowPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		leftArrowPressed = false;
		rightArrowPressed = true;
	}
	else
	{
		rightArrowPressed = false;
	}
}
void InfoDump()
{
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);

	EngineInfo engineInfo;
	engineInfo.LogEngineInfo();
	string displayAdaptor(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	string glVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	Console.PushLine("Display Adaptor: " + displayAdaptor + ", OpenGL Version " + glVersion + ", " + to_string(sysInfo.dwNumberOfProcessors) + " CPU threads avaliable.");
}
struct
{
	double x;
	double y;
} MousePos;
void RotateHandler(Transform transform, Shader shader)
{
	mat4 transformMatrix = mat4(1.0f);
	float MouseX, MouseY;
	if (LookAtMouse)
	{
		MouseX = -MousePos.y / 3;
		MouseY = -MousePos.x / 3;
	}
	else
	{
		MouseX = 0;
		MouseY = 0;
	}
	if (upArrowPressed)
	{
		xAngle += frameTime * 200;
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseX + xAngle, vec3(1, 0, 0));
	}
	else if (downArrowPressed)
	{
		xAngle -= frameTime * 200;
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseX + xAngle, vec3(1, 0, 0));
	}
	else
	{
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseX + xAngle, vec3(1, 0, 0));
	}
	if (leftArrowPressed)
	{
		yAngle += frameTime * 200;
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseY + yAngle, vec3(0, 1, 0));
	}
	else if (rightArrowPressed)
	{
		yAngle -= frameTime * 200;
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseY + yAngle, vec3(0, 1, 0));
	}
	else
	{
		transformMatrix = transform.RotateMatrix(transformMatrix, MouseY + yAngle, vec3(0, 1, 0));
	}
	shader.setMatrix("transform", transformMatrix);
}
void GenErrorTex(unsigned char* ErrorTex, int texture)
{
	if (ErrorTex)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, ErrorTex);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
}
int main()
{
	//--------------------------//
	// Initialises Window and GLFW
	//--------------------------//

	Transform transform;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit())
	{
		Console.PushError("Failed to initialise GLFW.");
	}

	GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		Console.PushError("Window creation failed for unknown reason.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Console.PushError("Failed to initialize GLAD");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//-------------//
	// Window icon
	//-------------//

	GLFWimage images[1];
	images[0].pixels = stbi_load("IMG_4766.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
	InfoDump();

	//---------------------//
	// Creates shader program
	//---------------------//

	Shader shader("VERTEX_SHADER.glsl", "FRAGMENT_SHADER.glsl");

	//------------------------------------------------------------//
	// Configuration of Vertex Array Object and Vertex Buffer Object
	//------------------------------------------------------------//

	unsigned int VAO, VBO, EBO, texture, texture2, ErrorTexture; 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenTextures(1, &ErrorTexture);
	glGenTextures(1, &texture);
	glGenTextures(1, &texture2);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	

	int width, height, nrChannels;
	int width2, height2, nrChannels2;
	int ErrorW, ErrorH, ErrorNrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* ErrorTex = stbi_load("EngineResources/ERROR.bmp", &ErrorW, &ErrorH, &ErrorNrChannels, 4);
	unsigned char* imageData = stbi_load("IMG_4766.png", &width, &height, &nrChannels, 4);
	//unsigned char* imageData2 = stbi_load("image.png", &width2, &height2, &nrChannels2, 4);

	nrChannels = 4;

	//-------------------------------------------------//
	// Define Texture wrapping modes for X , Y and Z axis
	//-------------------------------------------------//

	GLint MODE_X = GL_REPEAT;
	GLint MODE_Y = GL_REPEAT;
	GLint MODE_Z = GL_REPEAT;


	if (imageData)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MODE_X);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MODE_Y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		GenErrorTex(ErrorTex, texture);
		Console.PushError("Texture not found or improperly loaded!");
	}

	
	/*
	if (imageData2)
	{
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MODE_X);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MODE_Y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		std::cout << "Image data (texture 2) not correctly loaded!\n";
	}*/
	stbi_image_free(imageData);
	//stbi_image_free(imageData2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float incrementValue = 1.0f;
	incrementValue *= 0.001f;
	float mixValue = 0.0f;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{

		//----------------------------------------------------------------------------------------------------//
		//                                       BEGGINING OF FRAME
		//----------------------------------------------------------------------------------------------------//
		//cout << transform.VecToString(vec3(MousePos, 0)) << "\n";
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		MousePos.x = x;
		MousePos.y = y;
		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{
			//---------------//
			// handling inputs
			//---------------//

			processInput(window);

			//----------------------//
			// rendering commands here
			//----------------------//
			//glClearColor(1, 0, 0.75f, 1);
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			glBindVertexArray(VAO);


			float sine_time = sin(glfwGetTime()) + 1;
			float cosine_time = cos(glfwGetTime()) + 1;

			mixValue = clamp(mixValue, 0, 1);

			shader.use();
			//Vertex offset
			shader.setVector("offset", 0, 0, 0, sine_time);
			shader.setInt("MinionTexture", 0);
			shader.setBool("wireframe", inWireframe);
			shader.setInt("OtherTexture", 1);

			shader.setFloat("vertTruncAmount", vertTruncAmount);
			shader.setBool("truncVerts", truncVerts);
			glDrawElements(GL_TRIANGLES, sizeof(vertexData), GL_UNSIGNED_INT, 0);

			//-----------------------------------------//
			// Check and call events and swap the buffers
			// (Front buffer is current frame, swaps to
			// back buffer. Front buffer is cleared and
			// becomes the new back buffer.)
			//-----------------------------------------//

			glfwSwapBuffers(window);
			glfwPollEvents();

			if (!ambientRotation)
			{
				RotateHandler(transform, shader);
			}
			else
			{
				RotateHandler(transform, shader);
				mat4 transformMatrix = mat4(1.0f);
				xAngle = xAngle + frameTime * ambientRotationMultiplier;
				yAngle = yAngle + frameTime * -ambientRotationMultiplier;
				transformMatrix = transform.RotateMatrix(transformMatrix, xAngle, vec3(1, 0, 0));
				transformMatrix = transform.RotateMatrix(transformMatrix, yAngle, vec3(0, 1, 0));
				shader.setMatrix("transform", transformMatrix);
			}
			//std::cout << mixValue << "\n";
			endOfFrameTime = glfwGetTime();
			frameTime = endOfFrameTime - endOfFrameTimeLastFrame;
			frameRate = 1 / frameTime;
			//cout << "Frametime: " << frameTime << ", Framerate: " << frameRate << "\n";
			//cout << "Framerate: " << frameRate << "\n";

			//----------------------------------------------------------------------------------------------------//
			//                                            END OF FRAME
			//----------------------------------------------------------------------------------------------------//	

			endOfFrameTimeLastFrame = endOfFrameTime;
		}

	}
	glfwTerminate();

	return 0;
}
