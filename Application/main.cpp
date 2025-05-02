
//Some specific OpenGL stuff
#include "Libraries/Include/glad/glad.h"
#include "Libraries/Include/glfw3.h"

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
#include <vector>


//OpenGL Mathematics
#include "Libraries/Include/glm/glm.hpp"
#include "Libraries/Include/glm/gtc/matrix_transform.hpp"
#include "Libraries/Include/glm/gtc/type_ptr.hpp"

using namespace glm;

typedef unsigned char Byte;
typedef Byte cs_byte;

EngineInfo engineInfo;
WindowConsole Console;

vec2 windowSize(1200, 800);

float vertTruncAmount = 10;
bool truncVerts = false;

std::string currentScene = "Null";

int frame = 0;
int generatedTexture = 0;
std::vector <Texture> globalTextures;
std::vector <ObjContainer*> objects;
int pointLightNum = 1;
int spotLightNum = 1;
bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, inWireframe = false;
bool ambientRotation = false;
bool LookAtMouse = false;
int ambientRotationMultiplier = 50;

float mouseYaw = -90.0f;
float mousePitch = 0.0f;
const float MouseSensitivity = 0.1f;
bool mouseClickedThisFrame = false;
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
bool useScreenRefreshRate = true;

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
std::string bool_str(bool b)
{
	if (std::to_string(b) == "0")
		return "False";
	else
		return "True";
}

void UpdateWindowSize(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if (width == 0 || height == 0)
		return;
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

std::string windowName = "3D Renderer (Prototype)";

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
	std::string displayAdaptor(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	std::string glVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	Console.Log("Display Adaptor: " + displayAdaptor + ", OpenGL Version " + glVersion + ", " + std::to_string(sysInfo.dwNumberOfProcessors) + " CPU threads avaliable.");
}
std::string parameter(std::string str)
{
	return str.substr(1, str.find_first_of(" ") - 1);
}
vec4 ParseVector(std::string vecstr)
{
	float x, y, z, w;
	int val;
	std::string x_s, y_s, z_s, w_s = "1.0";

	x_s = vecstr.substr(0, vecstr.find_first_of("f"));

	vecstr = vecstr.substr(vecstr.find_first_of("f") + 3, vecstr.size());
	y_s = vecstr.substr(0, vecstr.find_first_of("f"));

	vecstr = vecstr.substr(vecstr.find_first_of("f") + 3, vecstr.size());
	z_s = vecstr.substr(0, vecstr.find_first_of("f"));

	try {
		vecstr = vecstr.substr(vecstr.find_first_of("f") + 3, vecstr.size());
		w_s = vecstr.substr(0, vecstr.find_first_of("f"));	
	}
	catch (std::exception e) {}

	std::istringstream strx(x_s);
	strx >> x;
	std::istringstream stry(y_s);
	stry >> y;
	std::istringstream strz(z_s);
	strz >> z;
	std::istringstream strw(w_s);
	strw >> w;

	return vec4(x,y,z,w);
}
void ReadScene(std::string scenePath)
{
	std::ifstream file(scenePath);
	std::string scene;
	int objects = 1;
	if (file)
	{
		std::string modelPath = "n";
		ObjContainer* newObj = new ObjContainer;
		while (std::getline(file, scene))
		{
			try {
				if (scene.substr(1, 6) == "OBJECT") {
					Console.Log("Creating object #" + std::to_string(objects));
				}
				else if (parameter(scene) == "object")
				{
					newObj->name(scene.substr(8, scene.size()).c_str());
				}
				else if (parameter(scene) == "model")
				{
					modelPath = scene.substr(scene.find_first_of('"') + 1, scene.size() - 9);
				}
				else if (parameter(scene) == "modeltextureflipping")
				{
					bool val;
					if (scene.substr(scene.find_first_of(" ") + 1, scene.size()) == "true")
						val = true;
					else
						val = false;
					if (modelPath != "n")
						newObj->SetModel(modelPath, val);
				}
				else if (parameter(scene) == "position")
				{
					newObj->transform.position = ParseVector(scene.substr(10, scene.size()));
				}
				//end
				else if (scene == "#END") {
					Console.Log("Scene loaded");
					break;
				}
			}
			catch (std::exception e) {}
		}

	}
	file.close();

}
void SetActiveScene(std::string scenePath)
{
	currentScene = scenePath.substr(scenePath.find_last_of("/") + 1, scenePath.substr().length());
	currentScene = currentScene.substr(0, currentScene.find_last_of("."));
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Destroy();
	}
	objects.clear();
	ReadScene(scenePath);
}
int main()
{
	//--------------------------//
	// Initialises Window and GLFW
	//--------------------------//

	Transforms transform;
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit())
	{
		Console.PushError("Failed to initialise GLFW.");
	}

	GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, "Loading objects...", NULL, NULL);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (useScreenRefreshRate)
	{
		desiredFrametime = 1 / (mode->refreshRate + 1);
	}
	//glfwSetWIndow sets window position from top left corner of window. 
	//Given the screen is 1080p and the windiw is 1280x800, x padding is 320.
	//320 + 1280 + 320 = 1920, 320 is padding. 
	glfwSetWindowPos(window, (mode->width - windowSize.x) / 2 /* x padding */, (mode->height - windowSize.y) / 2 /* y padding */);
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
	SetWindowIcon("Images/IMG_4766.png", window);
	InfoDump();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//---------------------//
	// Creates shader program
	//---------------------//

	Shader litShader("VERTEX_SHADER.glsl", "LIGHTING_FRAGMENT_SHADER.glsl", "litShader");
	Shader lightGizmo("LIGHT_GIZMO_VERT.glsl", "LIGHT_GIZMO_FRAG.glsl", "lightGizmo");

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);

	SetActiveScene("Scenes/Scene.sc");
	Model sphere = Model("Objects/Primitives/Sphere.obj");

	ObjContainer lightCube("Gay porn");
	lightCube.light.enabled = true;
	lightCube.SetModel(sphere);
	lightCube.renderer.material.shader = &lightGizmo;
	lightCube.transform.scale = vec3(0.2f);
	lightCube.transform.position = vec3(0.0f, 6.0f, -1.0f);

	lightCube.light.direction = vec3(0.0f, -1.0f, 0.0f);
	lightCube.light.cutoff = 17.5;
	lightCube.light.outerCutoff = 20.5;
	lightCube.light.type = LightType.Point;
	lightCube.light.colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
/*
	ObjContainer lightCube2;
	lightCube2.name("lightCube2");
	lightCube2 = lightCube;
	lightCube2.transform.position += vec3(1.5f, 0.0f, -2.0f);
	lightCube2.light.colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	ObjContainer lightCube3;
	lightCube3 = lightCube;
	lightCube3.light.type = LightType.Point;
	lightCube3.name("lightCube3");
	lightCube3.transform.position += vec3(-1.5f, 0.0f, -2.0f);
	lightCube3.light.colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
	ObjContainer Sun;
	Sun = lightCube;
	Sun.name("Sun");
	Sun.light.ambient = vec3(0.05f);
	Sun.light.type = LightType.Directional;
	Sun.light.colour = vec4(1);
	Sun.transform.position += vec3(0, 4, 0);
	Sun.transform.scale *= 2;*/

	mainCamera.Position = vec3(0.0f, 4.0f, 0.0f);
	mainCamera.fov = 75;

	float lightDistanceToCam;
	int num{ 0 };
	float distance{ 0 };
	glfwSetWindowTitle(window, "Done!");
	while (!glfwWindowShouldClose(window))
	{
		UpdateWindowSize(window);
		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{
			//object.transform.position = vec3(0, 0, sin(glfwGetTime() * 5));
			orthoscopicMat = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, 0.1f, 100.0f);
			projectionMat = glm::perspective(glm::radians(mainCamera.fov), windowSize.x / windowSize.y, nearClipPlane, farClipPlane);

			mat4 view = mat4(1.0f);
			view = lookAt(mainCamera.Position, mainCamera.Position + mainCamera.forward, mainCamera.up);
			//---------------//
			// stuff
			//---------------//
			
			processInput(window);
			MousePos.x = clamp(MousePos.x, 0, windowSize.x, true);
			MousePos.y = clamp(MousePos.y, 0, windowSize.y, true);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			for (int i = 0; i < globalTextures.size(); i++)
			{
				try {
					if (frame == 0)
					{
						if (globalTextures[i].manualFiltering() == false) {
							globalTextures[i].filterMode(engineInfo.defaultFiltering);
						}
						globalTextures[i].Set();
					}
				}
				catch (std::exception e) { Console.PushError(e.what()); }
			}
			
			//-------------------------------------------------------------------------------//
			// Drawing objects and sending their respective properties to their shader program.
			//-------------------------------------------------------------------------------//
			for (int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->renderer.material.shader == nullptr)
				{
					objects[i]->renderer.material.shader = &litShader;
				}
				if (objects[i]->light.enabled)
				{
					litShader.use();
					litShader.setLight(objects[i]->light, objects[i]->transform.position);
				}
				if (true) {
					objects[i]->renderer.material.shader->use();
					objects[i]->renderer.material.shader->setMatrix("projection", projectionMat);
					objects[i]->renderer.material.shader->setMatrix("view", view);
					objects[i]->renderer.material.shader->setVector("cameraPos", mainCamera.Position.x, mainCamera.Position.y, mainCamera.Position.z);
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
						objects[i]->renderer.material.emissive,
						objects[i]->renderer.material.emissiveTex.id(),
						objects[i]->renderer.material.emissiveTex.texAssigned(),
						objects[i]->renderer.material.emissiveTex.error());
					if (objects[i]->active)
					{
						objects[i]->Draw(objects[i]->renderer.material.culling, objects[i]->renderer.material.shader, engineInfo.pi);
					}
				}
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
			if (objects.size() != 0) {

				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && mouseClickedThisFrame == false)
				{
					distance = 0;
					mouseClickedThisFrame = true;
					for (int i = 0; i < objects.size(); i++)
					{
						if (objects[i]->light.enabled && objects[i]->light.type != LightType.Ambient)
						{
							objects[i]->renderer.material.colour = vec4(1);
							float new_distance = Vector3.Distance(objects[i]->transform.position, mainCamera.Position);
							if (new_distance <= distance || distance == 0)
							{
								distance = new_distance;
								num = i;
							}
							else {
							}
						}
					}
					lightDistanceToCam = Vector3.Distance(objects[num]->transform.position, mainCamera.Position);
				}
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && mouseClickedThisFrame)
				{
					objects[num]->renderer.material.colour = vec4(1.0f);
					mouseClickedThisFrame = false;

				}
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				{
					objects[num]->renderer.material.colour = vec4(1.0f, 1.0f, ((sin(glfwGetTime() * 10) / 2) + 1), 1.0f);
					lightDistanceToCam = clamp((lightDistanceToCam + scrollDir / 10), 1, 20);
					vec3 camForward = mainCamera.Position + mainCamera.forward * lightDistanceToCam;
					objects[num]->transform.position = camForward;
					objects[num]->light.direction = mainCamera.forward;
				}
				else {
					lightDistanceToCam = Vector3.Distance(objects[num]->transform.position, mainCamera.Position);
				}
			}
			scrollDir = 0;
			glfwSwapBuffers(window);
			glfwPollEvents();
			if (frame == 0)
			{
				glfwSetWindowTitle(window, windowName.c_str());
			}
			
			frame++;
			endOfFrameTime = glfwGetTime();
			frameTime = endOfFrameTime - endOfFrameTimeLastFrame;
			frameRate = 1 / frameTime;
			//Console.Log(std::to_string(frameRate));
			//----------------------------------------------------------------------------------------------------//
			//                                            END OF FRAME
			//----------------------------------------------------------------------------------------------------//	
			endOfFrameTimeLastFrame = endOfFrameTime;
		}
	}
	glfwTerminate();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Destroy();
	}
	objects.clear();
	return 0;
}
