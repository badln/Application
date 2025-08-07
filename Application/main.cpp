
#include "main.h"
#include "Scene.h"
#include "EngineResources/Console.h"
#include "Gamepad.h";
using namespace glm;

//TODO :: Completely rewrite textures yuck

typedef unsigned char Byte;
typedef Byte cs_byte;

WindowConsole Console;

vec2 windowSize;
vec2 renderResolution = EngineInfo.renderResolution;

int frame = 0;
std::vector <Texture> globalTextures;
std::vector <ObjContainer*> objects;
std::vector <Shader*> shaders;
std::vector <Camera*> worldCameras;

Camera* mainCamera = NULL;
ObjContainer Scene;
Gamepad pad;

unsigned int lightPointArray;

float speedMultiplier = 30.0f;
int drawCalls = 0;
int spotLightNum, pointLightNum;
bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, inWireframe = false;

bool mouseClickedThisFrame = false;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
float scrollDir;
bool looking = true;

float desiredFrametime = 1 / (EngineInfo.desiredFramerate + 1);
double endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;
bool useScreenRefreshRate = false;

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

bool processPadInput(GLFWwindow* window, Gamepad* pad)
{
	if (glfwJoystickIsGamepad(pad->thisPad) && pad->padConnected == false)
	{
		//Controller connected
		pad->padName = glfwGetGamepadName(GLFW_JOYSTICK_1);

		Console.PushWarning("Pad '" + pad->padName + "' connected.");
		pad->padConnected = true;
	}
	else if (!glfwJoystickIsGamepad(pad->thisPad) && pad->padConnected)
	{
		//Controller disconnected
		Console.PushWarning("Pad '" + pad->padName + "' disconnected.");
		pad->padConnected = false;
	}
	return glfwGetGamepadState(pad->thisPad, &pad->padState);

}

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
	Scene.Init();
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

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);


	GLFWmonitor* monitor = NULL;
	switch (EngineInfo.currentWindowType)
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

	GLFWwindow* window = glfwCreateWindow(EngineInfo.windowSize.x, EngineInfo.windowSize.y, "Loading objects...", monitor, NULL);
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
	
	Shader screenShader("DEFAULT_VERTEX_SHADER.glsl", "POST_PROCESS.glsl", "screenShader");
	Shader DrawLightGizmo("GEOMETRY_VERT_SHADER.glsl", "GEOMETRY_FRAG_SHADER.glsl", "DrawLightGizmo", "GEOMETRY_SHADER.glsl");
	Shader litShader("VERTEX_SHADER.glsl", "LIGHTING_FRAGMENT_SHADER.glsl", "litShader");

	float lightPoint[] = {
		0.0f, 0.0f, 0.0f
	};
	unsigned int VBO, quadVAO, quadVBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &lightPointArray);
	glBindVertexArray(lightPointArray);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightPoint), &lightPoint, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);

	float screenQuad[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	SetActiveScene("Scenes/FMP.sc");
	
	/*
	ObjContainer obj("Cube");
	obj.SetModel("Objects/Primitives/Cube.obj", true);
	obj.renderer.mesh.data.textures.push_back(Texture("Images/IMG_4766.png", TextureType::Diffuse));
*/
	//framebuffer texture, used for render resolution
	Framebuffer fb;
	Texture fbTexture(TextureType::Render);
	fb.Create(&fbTexture, GL_FRAMEBUFFER, "Test");
	EngineInfo.renderResolution = fb.texture->size();

	float lightDistanceToCam;
	int num{ 0 };
	float distance{ 0 };
	glfwSetWindowTitle(window, "Done!");
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window))
	{
		vec4 col = mainCamera->clearColour;
		glClearColor(col.r, col.g, col.b, col.a);
		UpdateWindowSize(window);
		windowSize = EngineInfo.windowSize;
		if (glfwGetTime() > endOfFrameTime + desiredFrametime)
		{
			orthoscopicMat = glm::ortho(0.0f, EngineInfo.renderResolution.x, 0.0f, EngineInfo.renderResolution.y, 0.1f, 100.0f);
			projectionMat = glm::perspective(glm::radians(mainCamera->fov), EngineInfo.renderResolution.x / EngineInfo.renderResolution.y, mainCamera->nearClipPlane, mainCamera->farClipPlane);

			mat4 view = mat4(1.0f);
			view = lookAt(mainCamera->Position, mainCamera->Position + mainCamera->forward, mainCamera->up);

			//---------------//
			// stuff
			//---------------//

			renderResolution = EngineInfo.renderResolution;
			processInput(window);
			EngineInfo.MousePos.x = clamp(EngineInfo.MousePos.x, 0, EngineInfo.renderResolution.x, true);
			EngineInfo.MousePos.y = clamp(EngineInfo.MousePos.y, 0, EngineInfo.renderResolution.y, true);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
			fb.use();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glViewport(0, 0, EngineInfo.renderResolution.x, EngineInfo.renderResolution.y);
			for (int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->renderer.material.shader == nullptr)
				{
					objects[i]->renderer.material.shader = &litShader;
				}
				if (true) {
					//TODO: fix this
					objects[i]->renderer.material.shader->use();
					objects[i]->renderer.material.shader->setMatrix("projection", projectionMat);
					objects[i]->renderer.material.shader->setMatrix("view", view);
					objects[i]->renderer.material.shader->setVector("cameraPos", mainCamera->Position.x, mainCamera->Position.y, mainCamera->Position.z);
					objects[i]->renderer.material.shader->setFloat("time", glfwGetTime());
					objects[i]->renderer.material.shader->setBool("wireframe", inWireframe);
					objects[i]->renderer.material.shader->setMaterial("mat", objects[i]->renderer.material.diffuse,
						objects[i]->renderer.material.specular,
						objects[i]->renderer.material.ambient,
						objects[i]->renderer.material.shininess,
						objects[i]->renderer.material.colour,
						objects[i]->renderer.material.texture.colour,
						(int)objects[i]->renderer.material.texture.id(),
						objects[i]->renderer.material.texture.texAssigned(),
						(int)objects[i]->renderer.material.diffuseTex.id(),
						objects[i]->renderer.material.diffuseTex.texAssigned(),
						(int)objects[i]->renderer.material.specularTex.id(),
						objects[i]->renderer.material.specularTex.texAssigned(),
						objects[i]->renderer.material.texture.error(),
						objects[i]->renderer.material.diffuseTex.error(),
						objects[i]->renderer.material.specularTex.error(),
						objects[i]->renderer.material.emissive,
						(int)objects[i]->renderer.material.emissiveTex.id(),
						objects[i]->renderer.material.emissiveTex.texAssigned(),
						objects[i]->renderer.material.emissiveTex.error());
					if (objects[i]->active)
					{
						if (objects[i]->light.enabled)
						{
							litShader.setLight(objects[i]->light, objects[i]->transform.position);
						}
						objects[i]->Draw(DrawLightGizmo, projectionMat, view, EngineInfo.renderResolution);
					}
				}
			}
			fb.use(0);
			glViewport(0, 0, EngineInfo.windowSize.x, EngineInfo.windowSize.y);
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT);
			screenShader.use();
			screenShader.setInt("tex", (int)fb.texture->id());
			screenShader.setVector("colour", 0, 0, 0, 0);
			screenShader.setFloat("time", glfwGetTime());
			glBindVertexArray(quadVAO);
			if (inWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBindTexture(GL_TEXTURE_2D, fb.texture->id());
			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (inWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			processPadInput(window, &pad);
			mainCamera->SetCameraDir(window, pad.RS_X_ADDITIVE, pad.RS_Y_ADDITIVE, (pad.RIGHT_STICK(EngineInfo.frameTime).x != 0 || pad.RIGHT_STICK(EngineInfo.frameTime).y != 0), true, pad.Sensitivity * EngineInfo.frameTime);
			mainCamera->Position += (speedMultiplier * (float)EngineInfo.frameTime) * mainCamera->forward * ( - pad.LEFT_STICK(EngineInfo.frameTime).y);
			mainCamera->Position -= normalize(cross(mainCamera->forward, mainCamera->up)) * (speedMultiplier * (float)EngineInfo.frameTime) * -pad.LEFT_STICK(EngineInfo.frameTime).x;


			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				mainCamera->Position += (speedMultiplier * (float)EngineInfo.frameTime) * mainCamera->forward;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				mainCamera->Position -= (speedMultiplier * (float)EngineInfo.frameTime) * mainCamera->forward;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				mainCamera->Position -= mainCamera->left * (speedMultiplier * (float)EngineInfo.frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				mainCamera->Position += mainCamera->left * (speedMultiplier * (float)EngineInfo.frameTime);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || pad.padState.buttons[GLFW_GAMEPAD_BUTTON_A])
			{
				mainCamera->Position += (speedMultiplier * (float)EngineInfo.frameTime) * normalize(cross(normalize(cross(mainCamera->forward, mainCamera->up)), mainCamera->forward));
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || pad.padState.buttons[GLFW_GAMEPAD_BUTTON_B])
			{
				mainCamera->Position -= (speedMultiplier * (float)EngineInfo.frameTime) * normalize(cross(normalize(cross(mainCamera->forward, mainCamera->up)), mainCamera->forward));
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
					Console.Log("Light pos: ", false);
					Console.Log(objects[num]->transform.position);
					Console.Log("light dir: ", false);
					Console.Log(objects[num]->light.direction);
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
			//Console.Log("Draw calls this frame: " + std::to_string(drawCalls));
			drawCalls = 0;
			frame++;
			endOfFrameTime = glfwGetTime();
			EngineInfo.frameTime = endOfFrameTime - endOfFrameTimeLastFrame;
			frameRate = 1 / EngineInfo.frameTime;
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
