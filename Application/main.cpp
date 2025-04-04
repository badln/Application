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

EngineInfo engineInfo;
WindowConsole Console;

vec2 windowSize(650, 650);

//Scene colours

LightSource globalSceneLight;
LightSource lightSource1;

vec3 cubeData[]{
	//     Positions        |         Scales        |       Rotations
	vec3( 1.5f, 0.0f, -4.0f), vec3(5.0f, 5.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f),
	vec3(-0.5f, 0.0f, -3.0f), vec3(5.0f, 5.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f),
};
float vertTruncAmount = 10;
bool truncVerts = false;

bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, inWireframe = false;
bool ambientRotation = false;
bool LookAtMouse = false;
bool textureError = false;
int ambientRotationMultiplier = 50;

float mouseYaw = -90.0f;
float mousePitch = 0.0f;
const float MouseSensitivity = 0.1f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float xpos_LF = windowSize.x / 2;
float ypos_LF = windowSize.y / 2;
bool firstMouse = true;
bool looking = true;

float nearClipPlane = 0.1f;
float farClipPlane = 100.0f;
float fieldOfView = 45;
float desiredFrametime = 1 / engineInfo.desiredFramerate;
double frameTime, endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;

mat4 orthoscopicMat;
mat4 projectionMat;

void DoAnimation(bool flag)
{
	if (!flag)
	{
		return;
	}
	cubeData[6].x += frameTime;
	cubeData[9].x += frameTime;
	cubeData[12].x += frameTime;

	if (cubeData[6].x > -1.85f && cubeData[3].y > -4.5f)
	{
		cubeData[3].y -= frameTime * 3;
		cubeData[4].y -= frameTime * 3;
	}

	if (cubeData[6].x > 0.15f && cubeData[0].y > -4.5f)
	{
		cubeData[0].y -= frameTime * 3;
		cubeData[1].y -= frameTime * 3;
	}

	if (cubeData[6].x > 4)
	{
		cubeData[0].y = 0;
		cubeData[1].y = 5;
		cubeData[3].y = 0;
		cubeData[4].y = 5;
		cubeData[6].x = -4.0f;
		cubeData[9].x = -3.75f;
		cubeData[12].x = -5.0f;
	}

}


struct {
	vec3 zero = vec3(0.0f);
	vec3 one = vec3(1.0f);

	vec3 forward = vec3(0.0f, 0.0f, 1.0f);
	vec3 backward = vec3(0.0f, 0.0f, -1.0f);

	vec3 left = vec3(1.0f, 0.0f, 0.0f);
	vec3 right = vec3(-1.0f, 0.0f, 0.0f);

	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 down = vec3(0.0f, -1.0f, 0.0f);
}Vector3;

struct {

	vec3 Position = vec3(0.0f, 0.0f, 3.0f);

	vec3 Target = Vector3.zero;

	vec3 Direction = normalize(Position - Target);

	float SpeedMultiplier = 3;
	vec3 zero = vec3(0.0f);
	vec3 one = vec3(1.0f);

	vec3 forward = vec3(0.0f, 0.0f, -1.0f);
	vec3 up =	   vec3(0.0f, 1.0f, 0.0f);
	vec3 right =   vec3(1.0f, 0.0f, 0.0f);
	vec3 lookDirection;

} Camera ;

float AddRotation, xAngle, yAngle = 0;

float vertexData[] = {

	//---------------------------------------------------//
	// Vertex Position  |   Vertex Normals   | TexCoords
	//---------------------------------------------------//
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
   	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

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

void UpdateWindowSize(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	windowSize.x = width;
	windowSize.y = height;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!looking)
	{
		return;
	}
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		xpos_LF = xpos;
		ypos_LF = ypos;
		firstMouse = false;
	}
	float xOffset = xpos - xpos_LF;
	float yOffset = -(ypos - ypos_LF);

	xpos_LF = xpos;
	ypos_LF = ypos;

	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	mouseYaw += xOffset;
	mousePitch += yOffset;

	mousePitch = clamp(mousePitch, -89.0f, 89.0f);

	Camera.lookDirection.x = cos(radians(mouseYaw)) * cos(radians(mousePitch));
	Camera.lookDirection.y = sin(radians(mousePitch));
	Camera.lookDirection.z = sin(radians(mouseYaw)) * cos(radians(mousePitch));
	Camera.forward = normalize(Camera.lookDirection);
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
	if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) && escPressed == false)
	{
		//glfwSetWindowShouldClose(window, true);
		escPressed = true;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			looking = false;
		}
		else {
			firstMouse = true;
			looking = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		escPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		ctrlPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && ctrlPressed == false)
	{
		//------------------------------------//
		// Toggles wireframe rendering on or off
		//------------------------------------//
		ctrlPressed = true;
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
mat4 RotateHandler(Transform transform, Shader shader)
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
	return transformMatrix;
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
		textureError = true;
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
	images[0].pixels = stbi_load("image/IMG_4766.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
	InfoDump();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//---------------------//
	// Creates shader program
	//---------------------//

	Shader lightingShader("VERTEX_SHADER.glsl", "LIGHTING_FRAGMENT_SHADER.glsl");

	//------------------------------------------------------------//
	// Configuration of Vertex Array Object and Vertex Buffer Object
	//------------------------------------------------------------//

	unsigned int lightVAO, VAO, VBO, EBO, texture, texture2, ErrorTexture;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenTextures(1, &ErrorTexture);
	glGenTextures(1, &texture);
	glGenTextures(1, &texture2);

	glBindVertexArray(VAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

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
	unsigned char* imageData = stbi_load("image/IMG_4766.png", &width, &height, &nrChannels, 4);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		GenErrorTex(ErrorTex, texture);
		Console.PushError("Texture not found or improperly loaded!");
	}

	stbi_image_free(imageData);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float incrementValue = 1.0f;
	incrementValue *= 0.001f;

	glEnable(GL_DEPTH_TEST);

	globalSceneLight.colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	globalSceneLight.position = vec3(0.0f, 0.0f, 0.0f);
	globalSceneLight.rotation = vec3(0.8f, 1.3f, 0.0f);

	Material mat;
	mat.ambient = 0.1f;
	mat.diffuse = 1;
	mat.shininess = 1;
	mat.specular = 0.5f;
	mat.colour = vec4(1.0f, 1.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{

		//----------------------------------------------------------------------------------------------------//
		//                                       BEGGINING OF FRAME
		//----------------------------------------------------------------------------------------------------//

		UpdateWindowSize(window);

		orthoscopicMat = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, 0.1f, 100.0f);
		projectionMat = glm::perspective(glm::radians(fieldOfView), windowSize.x / windowSize.y, nearClipPlane, farClipPlane);

		mat4 view = mat4(1.0f);
		view = lookAt(Camera.Position, Camera.Position + Camera.forward, Camera.up);

		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{
			globalSceneLight.position.x = sin(glfwGetTime()) * 10;
			//globalSceneLight.position = Camera.Position;
			//---------------//
			// handling inputs
			//---------------//

			processInput(window);

			//----------------------//
			// rendering commands here
			//----------------------//
			//glClearColor(1, 0, 0.75f, 1);
			glClearColor(globalSceneLight.colour.x / 15 * globalSceneLight.colour.w, globalSceneLight.colour.y / 15 * globalSceneLight.colour.w, globalSceneLight.colour.z / 15 * globalSceneLight.colour.w, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindVertexArray(VAO);
			glBindVertexArray(lightVAO);

			/*
			float rainbowTime = 10.0f;
			globalSceneLight.x = (sin((glfwGetTime() * rainbowTime) + 1) + 1) / 2;
			globalSceneLight.y = (sin((glfwGetTime() * rainbowTime) + 3) + 1) / 2;
			globalSceneLight.z = (sin((glfwGetTime() * rainbowTime) + 5) + 1) / 2;

			DoAnimation(true);
			*/

			lightingShader.use();

			lightingShader.setMatrix("perspective", projectionMat);
			lightingShader.setMatrix("view", view);

			lightingShader.setInt("Texture", 0);
			lightingShader.setVector("globalSceneLight", globalSceneLight.colour.x, globalSceneLight.colour.y, globalSceneLight.colour.z, globalSceneLight.colour.w);
			lightingShader.setVector("globalSceneLightPos", globalSceneLight.position.x, globalSceneLight.position.y, globalSceneLight.position.z);
			lightingShader.setVector("cameraPos", Camera.Position.x, Camera.Position.y, Camera.Position.z);

			lightingShader.setMaterial("mat", mat);

			lightingShader.setBool("wireframe", inWireframe);
			lightingShader.setInt("OtherTexture", 1);

			lightingShader.setFloat("vertTruncAmount", vertTruncAmount);
			lightingShader.setBool("truncVerts", truncVerts);
			lightingShader.setBool("error", textureError);

			lightingShader.setFloat("time", glfwGetTime());


			mat4 transformMatrix = mat4(1.0f);

			for (int i = 0; i < ((sizeof(cubeData) / sizeof(cubeData[0])) / 3); i++)
			{

				mat4 model = transformMatrix;

				model = transform.TranslateMatrix(model, cubeData[(-3 + ((i + 1) * 3))]);

				model = rotate(model, cubeData[(-1 + ((i + 1) * 3))].x * (engineInfo.pi / 180), vec3(1.0f, 0.0f, 0.0f));
				model = rotate(model, cubeData[(-1 + ((i + 1) * 3))].y * (engineInfo.pi / 180), vec3(0.0f, 1.0f, 0.0f));
				model = rotate(model, cubeData[(-1 + ((i + 1) * 3))].z * (engineInfo.pi / 180), vec3(0.0f, 0.0f, 1.0f));

				model = scale(model, cubeData[(-2 + ((i + 1) * 3))]);

				lightingShader.setMatrix("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				Camera.Position += (Camera.SpeedMultiplier * (float)frameTime) * Camera.forward;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				Camera.Position -= (Camera.SpeedMultiplier * (float)frameTime) * Camera.forward;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				Camera.Position -= normalize(cross(Camera.forward, Camera.up)) * (Camera.SpeedMultiplier * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				Camera.Position += normalize(cross(Camera.forward, Camera.up)) * (Camera.SpeedMultiplier * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				Camera.Position += (Camera.SpeedMultiplier * (float)frameTime) * Camera.up;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				Camera.Position -= (Camera.SpeedMultiplier * (float)frameTime) * Camera.up;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				globalSceneLight.colour.w -= 1.0f * frameTime;
			}
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				globalSceneLight.colour.w += 1.0f * frameTime;
			}

			glfwSwapBuffers(window);
			glfwPollEvents();

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
