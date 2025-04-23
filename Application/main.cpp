
//Some specific OpenGL stuff
#include "GLFW\Include\glad\glad.h"
#include "GLFW\Include\glfw3.h"

//general stuff
#include <iostream>
#include <fstream>
#include <sstream>
#include "objects.h"
#include "engine.h";
#include "transform.h"
#include <windows.h>
#include <list>
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

float vertTruncAmount = 10;
bool truncVerts = false;

int frame = 0;
int generatedTexture = -1;
vector <Texture*> textures;
vector <unsigned int> VAOs;
vector <ObjContainer*> objects;
bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, inWireframe = false;
bool ambientRotation = false;
bool LookAtMouse = false;
int ambientRotationMultiplier = 50;

float mouseYaw = -90.0f;
float mousePitch = 0.0f;
const float MouseSensitivity = 0.1f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
float scrollDir;
float xpos_LF = windowSize.x / 2;
float ypos_LF = windowSize.y / 2;
bool firstMouse = true;
bool looking = true;

float nearClipPlane = 0.1f;
float farClipPlane = 100.0f;
float desiredFrametime = 1 / (engineInfo.desiredFramerate + 1);
double frameTime, endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;

vec2 MousePos;

mat4 orthoscopicMat;
mat4 projectionMat;

Camera mainCamera;

struct {
	vec3 zero = vec3(0.0f);
	vec3 one = vec3(1.0f);

	vec3 forward = vec3(0.0f, 0.0f, 1.0f);
	vec3 backward = vec3(0.0f, 0.0f, -1.0f);

	vec3 left = vec3(1.0f, 0.0f, 0.0f);
	vec3 right = vec3(-1.0f, 0.0f, 0.0f);

	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 down = vec3(0.0f, -1.0f, 0.0f);

	float Distance(vec3 v, vec3 w)
	{
		float x = pow((v.x - w.x), 2); 
		float y = pow((v.y - w.y), 2);
		float z = pow((v.z - w.z), 2);

		return sqrt(x + y + z);
	}
}Vector3;

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
float clamp(float val, float min, float max, bool loop = false)
{
	if (val < min)
	{
		if (loop)
		{
			int c = (int)val / -max;
			val += max * (c + 1);
		}
		else
			val = min;
	}
	if (val > max)
	{
		if (loop)
		{
			int c = (int)val / max;
			val -= max * c;
		}
		else
			val = max;
	}
	return val;
}

string bool_str(bool b)
{
	if (to_string(b) == "0")
		return "False";
	else
		return "True";
}

void UpdateWindowSize(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	windowSize.x = width;
	windowSize.y = height;
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	scrollDir = yOffset;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!looking)
	{
		return;
	}
	MousePos.x = static_cast<float>(xposIn);
	MousePos.y = static_cast<float>(yposIn);

	if (firstMouse)
	{
		xpos_LF = MousePos.x;
		ypos_LF = MousePos.y;
		firstMouse = false;
	}
	float xOffset = MousePos.x - xpos_LF;
	float yOffset = -(MousePos.y - ypos_LF);

	xpos_LF = MousePos.x;
	ypos_LF = MousePos.y;

	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	mouseYaw += xOffset;
	mousePitch += yOffset;

	mousePitch = clamp(mousePitch, -89.0f, 89.0f);

	mainCamera.lookDirection.x = cos(radians(mouseYaw)) * cos(radians(mousePitch));
	mainCamera.lookDirection.y = sin(radians(mousePitch));
	mainCamera.lookDirection.z = sin(radians(mouseYaw)) * cos(radians(mousePitch));
	mainCamera.forward = normalize(mainCamera.lookDirection);
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
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
	Console.Log("Display Adaptor: " + displayAdaptor + ", OpenGL Version " + glVersion + ", " + to_string(sysInfo.dwNumberOfProcessors) + " CPU threads avaliable.");
}
int main()
{
	//--------------------------//
	// Initialises Window and GLFW
	//--------------------------//

	Transforms transform;
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
	SetWindowIcon("image/IMG_4766.png", window);
	InfoDump();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//---------------------//
	// Creates shader program
	//---------------------//

	Shader litShader("VERTEX_SHADER.glsl", "LIGHTING_FRAGMENT_SHADER.glsl", "litShader");
	Shader lightGizmo("LIGHT_GIZMO_VERT.glsl", "LIGHT_GIZMO_FRAG.glsl", "lightGizmo");

	//------------------------------------------------------------//
	// Configuration of Vertex Array Object and Vertex Buffer Object
	//------------------------------------------------------------//

	unsigned int lightVAO, VBO, EBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glEnable(GL_DEPTH_TEST);

	Mesh cubeMesh;
	cubeMesh.Set(vertexData, sizeof(vertexData));

	ObjContainer lightCube("LightSource");
	lightCube.renderer.mesh = cubeMesh;
	lightCube.renderer.material.shader = &lightGizmo;
	lightCube.transform.scale = vec3(0.2f);

	lightCube.transform.position = vec3(0.0f, 3.0f, -3.0f);

	lightCube.light.direction = vec3(0.0f, -1.0f, 0.0f);
	lightCube.light.type = 0;

	lightCube.light.linear = 0.045f;
	lightCube.light.quadratic = 0.0075;

	lightCube.light.cutoff = 17.5;
	lightCube.light.outerCutoff = 20.5;

	ObjContainer object;
	object.renderer.material.diffuseTex = Texture("image/emerald_ore/emerald_ore.png");
	object.renderer.material.specularTex = Texture("image/emerald_ore/emerald_ore_s.png");
	object.renderer.material.emmissiveTex = Texture("image/emerald_ore/emerald_ore_e.png");
	object.renderer.mesh = cubeMesh;

	object.transform.scale = vec3(5.0f);
	object.transform.position = vec3(0.0f, 0.0f, -3.0f);

	mainCamera.Position = vec3(0.0f, 4.0f, 0.0f);

	mainCamera.fov = 75;
	float lightDistanceToCam;
	while (!glfwWindowShouldClose(window))
	{

		//----------------------------------------------------------------------------------------------------//
		//                                       BEGGINING OF FRAME
		//----------------------------------------------------------------------------------------------------//
		UpdateWindowSize(window);

		orthoscopicMat = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, 0.1f, 100.0f);
		projectionMat = glm::perspective(glm::radians(mainCamera.fov), windowSize.x / windowSize.y, nearClipPlane, farClipPlane);

		mat4 view = mat4(1.0f);
		view = lookAt(mainCamera.Position, mainCamera.Position + mainCamera.forward, mainCamera.up);

		lightCube.renderer.material.colour = lightCube.light.colour;

		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{
			//---------------//
			// stuff
			//---------------//

			processInput(window);
			MousePos.x = clamp(MousePos.x, 0, windowSize.x, true);
			MousePos.y = clamp(MousePos.y, 0, windowSize.y, true);

			//----------------------//
			// rendering commands here
			//----------------------//
			glClearColor(lightCube.light.colour.x / 15 * lightCube.light.colour.w, lightCube.light.colour.y / 15 * lightCube.light.colour.w, lightCube.light.colour.z / 15 * lightCube.light.colour.w, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < generatedTexture + 1; i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, i + 1);
				if (frame == 0)
				{
					if (!textures[i]->manualFiltering()) {
						textures[i]->filterMode(engineInfo.defaultFiltering);
					}
					textures[i]->Set();
				}
			}
			
			for (int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->renderer.material.shader == NULL)
				{
					objects[i]->renderer.material.shader = &litShader;
				}
				objects[i]->renderer.material.shader->use();
				objects[i]->renderer.material.shader->setMatrix("projection", projectionMat);
				objects[i]->renderer.material.shader->setMatrix("view", view);
				objects[i]->renderer.material.shader->setVector("cameraPos", mainCamera.Position.x, mainCamera.Position.y, mainCamera.Position.z);
				objects[i]->renderer.material.shader->setLight("light", lightCube.light, lightCube.transform.position);
				objects[i]->renderer.material.shader->setFloat("time", glfwGetTime());
				objects[i]->renderer.material.shader->setBool("wireframe", inWireframe);
				objects[i]->renderer.material.shader->setVector("colour", objects[i]->renderer.material.colour.x, objects[i]->renderer.material.colour.y, objects[i]->renderer.material.colour.z);
				objects[i]->renderer.material.shader->setMaterial("mat", objects[i]->renderer.material.diffuse,
					objects[i]->renderer.material.specular,
					objects[i]->renderer.material.ambient,
					objects[i]->renderer.material.shininess,
					objects[i]->renderer.material.colour,
					objects[i]->renderer.material.texture.colour,
					objects[i]->renderer.material.texture.id(),
					objects[i]->renderer.material.texture.texAssigned(),
					objects[i]->renderer.material.diffuseTex.id(),
					objects[i]->renderer.material.diffuseTex.texAssigned(),
					objects[i]->renderer.material.specularTex.id(),
					objects[i]->renderer.material.specularTex.texAssigned(),
					objects[i]->renderer.material.texture.error(),
					objects[i]->renderer.material.diffuseTex.error(),
					objects[i]->renderer.material.specularTex.error(),
					objects[i]->renderer.material.emmissive,
					objects[i]->renderer.material.emmissiveTex.id(),
					objects[i]->renderer.material.emmissiveTex.texAssigned(),
					objects[i]->renderer.material.emmissiveTex.error());
				mat4 model = mat4(1.0f);
				model = translate(model, objects[i]->transform.position + objects[i]->transform.localPosition);
				model = rotate(   model, objects[i]->transform.rotation.x * (engineInfo.pi / 180), vec3(1.0f, 0.0f, 0.0f));
				model = rotate(   model, objects[i]->transform.rotation.x * (engineInfo.pi / 180), vec3(0.0f, 1.0f, 0.0f));
				model = rotate(   model, objects[i]->transform.rotation.x * (engineInfo.pi / 180), vec3(0.0f, 0.0f, 1.0f));
				model = scale(	  model, objects[i]->transform.scale);
				objects[i]->renderer.material.shader->setMatrix("model", model);
				glBindVertexArray(objects[i]->renderer.mesh.VAO());
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				mainCamera.Position += (mainCamera.SpeedMultiplier * (float)frameTime) * mainCamera.forward;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				mainCamera.Position -= (mainCamera.SpeedMultiplier * (float)frameTime) * mainCamera.forward;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				mainCamera.Position -= normalize(cross(mainCamera.forward, mainCamera.up)) * (mainCamera.SpeedMultiplier * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				mainCamera.Position += normalize(cross(mainCamera.forward, mainCamera.up)) * (mainCamera.SpeedMultiplier * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				mainCamera.Position += (mainCamera.SpeedMultiplier * (float)frameTime) * mainCamera.up;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				mainCamera.Position -= (mainCamera.SpeedMultiplier * (float)frameTime) * mainCamera.up;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				lightCube.light.colour.w -= 1.0f * frameTime;
			}
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				lightCube.light.colour.w += 1.0f * frameTime;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				lightDistanceToCam = clamp((lightDistanceToCam + scrollDir / 10), 1, 20);
				vec3 camForward = mainCamera.Position + mainCamera.forward * lightDistanceToCam;
				lightCube.transform.position = camForward;
				lightCube.light.direction = mainCamera.forward;
			}
			else {
				lightDistanceToCam = Vector3.Distance(lightCube.transform.position, mainCamera.Position);
			}
			scrollDir = 0;
			glfwSwapBuffers(window);
			glfwPollEvents();

			frame++;
			endOfFrameTime = glfwGetTime();
			frameTime = endOfFrameTime - endOfFrameTimeLastFrame;
			frameRate = 1 / frameTime;
			//Console.Log(to_string(frameRate));
			//----------------------------------------------------------------------------------------------------//
			//                                            END OF FRAME
			//----------------------------------------------------------------------------------------------------//	
			endOfFrameTimeLastFrame = endOfFrameTime;
		}
	}
	
	for (int i = 0; i < VAOs.size(); i++)
	{
		glDeleteVertexArrays(1, &VAOs[i]);
	}

	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();

	return 0;
}
