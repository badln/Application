
#include "main.h"
#include "Scene.h"
#include "EngineResources/Console.h"
using namespace glm;

typedef unsigned char Byte;
typedef Byte cs_byte;

WindowConsole Console;

float vertTruncAmount = 10;
bool truncVerts = false;

int frame = 0;
int generatedTexture = 0;
std::vector <Texture> globalTextures;
std::vector <ObjContainer*> objects;
std::vector <Shader*> shaders;
std::vector <Camera*> worldCameras;
Camera* mainCamera = NULL;

unsigned int lightPointArray;

int pointLightNum = 1;
int spotLightNum = 1;
bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, inWireframe = false;
bool ambientRotation = false;
bool LookAtMouse = false;
int ambientRotationMultiplier = 50;

bool mouseClickedThisFrame = false;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
float scrollDir;
bool looking = true;

float desiredFrametime = 1 / (EngineInfo.desiredFramerate + 1);
double frameTime, endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;
bool useScreenRefreshRate = true;

mat4 orthoscopicMat;
mat4 projectionMat;

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
	EngineInfo.windowSize.x = width;
	EngineInfo.windowSize.y = height;
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	scrollDir = yOffset;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	mainCamera->SetCameraDir(window, xposIn, yposIn, looking);
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
			mainCamera->firstCamFrame = true;
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

	EngineInfo.LogEngineInfo();
	std::string displayAdaptor(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	std::string glVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	Console.Log("Display Adaptor: " + displayAdaptor + ", OpenGL Version " + glVersion + ", " + std::to_string(sysInfo.dwNumberOfProcessors) + " CPU threads avaliable.");
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

	GLFWwindow* window = glfwCreateWindow(EngineInfo.windowSize.x, EngineInfo.windowSize.y, "Loading objects...", NULL, NULL);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (useScreenRefreshRate)
	{
		desiredFrametime = 1 / (mode->refreshRate + 1);
	}
	//glfwSetWIndow sets window position from top left corner of window. 
	//Given the screen is 1080p and the windiw is 1280x800, x padding is 320.
	//320 + 1280 + 320 = 1920, 320 is padding. 
	glfwSetWindowPos(window, (mode->width - EngineInfo.windowSize.x) / 2 /* x padding */, (mode->height - EngineInfo.windowSize.y) / 2 /* y padding */);
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

	Shader DrawLightGizmo("GEOMETRY_VERT_SHADER.glsl", "GEOMETRY_FRAG_SHADER.glsl", "DrawLightGizmo", "GEOMETRY_SHADER.glsl");
	Shader litShader("VERTEX_SHADER.glsl", "LIGHTING_FRAGMENT_SHADER.glsl", "litShader");

	float lightPoint[] = {
		0.0f, 0.0f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &lightPointArray);
	glBindVertexArray(lightPointArray);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightPoint), &lightPoint, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);

	SetActiveScene("Scenes/Scene.sc");

	float lightDistanceToCam;
	int num{ 0 };
	float distance{ 0 };
	glfwSetWindowTitle(window, "Done!");
	while (!glfwWindowShouldClose(window))
	{
		vec4 col = mainCamera->clearColour;
		glClearColor(col.r, col.g, col.b, col.a);
		UpdateWindowSize(window);
		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{

			orthoscopicMat = glm::ortho(0.0f, EngineInfo.windowSize.x, 0.0f, EngineInfo.windowSize.y, 0.1f, 100.0f);
			projectionMat = glm::perspective(glm::radians(mainCamera->fov), EngineInfo.windowSize.x / EngineInfo.windowSize.y, mainCamera->nearClipPlane, mainCamera->farClipPlane);

			mat4 view = mat4(1.0f);
			view = lookAt(mainCamera->Position, mainCamera->Position + mainCamera->forward, mainCamera->up);
			//---------------//
			// stuff
			//---------------//

			processInput(window);
			EngineInfo.MousePos.x = clamp(EngineInfo.MousePos.x, 0, EngineInfo.windowSize.x, true);
			EngineInfo.MousePos.y = clamp(EngineInfo.MousePos.y, 0, EngineInfo.windowSize.y, true);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			for (int i = 0; i < globalTextures.size(); i++)
			{
				try {
					if (frame == 0)
					{
						if (globalTextures[i].manualFiltering() == false) {
							globalTextures[i].filterMode(EngineInfo.defaultFiltering);
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
				if (true) {
					objects[i]->renderer.material.shader->use();
					objects[i]->renderer.material.shader->setMatrix("projection", projectionMat);
					objects[i]->renderer.material.shader->setMatrix("view", view);
					objects[i]->renderer.material.shader->setVector("cameraPos", mainCamera->Position.x, mainCamera->Position.y, mainCamera->Position.z);
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
						if (objects[i]->light.enabled)
						{
							litShader.setLight(objects[i]->light, objects[i]->transform.position);
						}
						objects[i]->Draw(DrawLightGizmo, projectionMat, view);
					}
				}
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				mainCamera->Position += (3 * (float)frameTime) * mainCamera->forward;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				mainCamera->Position -= (3 * (float)frameTime) * mainCamera->forward;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				mainCamera->Position -= normalize(cross(mainCamera->forward, mainCamera->up)) * (3 * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				mainCamera->Position += normalize(cross(mainCamera->forward, mainCamera->up)) * (3 * (float)frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				mainCamera->Position += (3 * (float)frameTime) * mainCamera->up;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				mainCamera->Position -= (3 * (float)frameTime) * mainCamera->up;
			}
			if (objects.size() != 0) {

				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && mouseClickedThisFrame == false)
				{
					distance = 0;
					mouseClickedThisFrame = true;
					for (int i = 0; i < objects.size(); i++)
					{
						if (objects[i]->light.enabled && objects[i]->light.type != LightType::Ambient)
						{
							float new_distance = Vector3.Distance(objects[i]->transform.position, mainCamera->Position);
							if (new_distance <= distance || distance == 0)
							{
								distance = new_distance;
								num = i;
							}
							else {
							}
						}
					}
					lightDistanceToCam = Vector3.Distance(objects[num]->transform.position, mainCamera->Position);
				}
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && mouseClickedThisFrame)
				{
					mouseClickedThisFrame = false;

				}
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				{
					objects[num]->renderer.material.colour = vec4(1.0f, 1.0f, ((sin(glfwGetTime() * 10) / 2) + 1), 1.0f);
					lightDistanceToCam = clamp((lightDistanceToCam + scrollDir / 10), 1, 20);
					vec3 camForward = mainCamera->Position + mainCamera->forward * lightDistanceToCam;
					objects[num]->transform.position = camForward;
					objects[num]->light.direction = mainCamera->forward;
				}
				else {
					lightDistanceToCam = Vector3.Distance(objects[num]->transform.position, mainCamera->Position);
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
		try {
			//objects[i]->Destroy();
		}
		catch (std::exception e) {}
	}
	objects.clear();
	return 0;
}
