
//compile with: /doc

#include "main.h"
#include "Scene.h"
#include "Gamepad.h";
#include "MainProgram.cpp"
#include "global.h"
#include "Entity.h"

using namespace glm;

//Defining global variables

Vector2 KTYGlobal::windowSize;
Vector2 KTYGlobal::renderSize;

const GLFWvidmode* KTYGlobal::_monitorInfo;

int KTYGlobal::_glMaxTextures;

int KTYGlobal::_frame;

bool KTYGlobal::_focus;

GLFWwindow* KTYGlobal::_mainWindow;

#pragma region Initialiser

MainProgram prog;

int main() {
	prog.Begin("KTYEngine Test");
}

#pragma endregion Initialiser

#pragma region SeaOfDeadCode
/*

typedef unsigned char Byte;
typedef Byte cs_byte;

WindowConsole Console;
vec2 renderResolution = EngineInfo.renderResolution;

int frame = 0;
std::vector <Texture> globalTextures;
std::vector <ObjContainer*> objects;
std::vector <Shader*> shaders;
std::vector <Camera*> worldCameras;
bool drawGizmos;

Camera* mainCamera = NULL;
ObjContainer Scene;
Gamepad pad;

unsigned int lightPointArray;
float speedMultiplier = 30.0f;
int drawCalls = 0;
int spotLightNum, pointLightNum;
bool downArrowPressed, upArrowPressed, leftArrowPressed, rightArrowPressed, spacePressed, ctrlPressed, escPressed, r3Pressed, inWireframe = false;

bool mouseClickedThisFrame = false;
bool looking = true;

float desiredFrametime = 1 / (EngineInfo.desiredFramerate + 1);
double endOfFrameTime, endOfFrameTimeLastFrame;
float frameRate;
bool useScreenRefreshRate = false;

mat4 orthoscopicMat;
mat4 projectionMat;
*/
/*
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

	InitWindow();


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

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// TODO : Completely rework scene loading, include the legacy scenes as an option

	SetActiveScene("Scenes/FMP.sc");
	ApplySettings();
	vec3 startPos = vec3(-75.635887f, 22.039347f, 85.790657f);
	

	Framebuffer fb;
	Texture fbTexture(TextureType::Render, EngineInfo.renderResolution);
	fb.Create(&fbTexture, GL_FRAMEBUFFER, "Test");
	EngineInfo.renderResolution = fb.texture->size();

	float lightDistanceToCam;
	int num{ 0 };
	float distance{ 0 };
	glfwSetWindowTitle(window, "Done!");
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	drawGizmos = EngineInfo.drawGizmos;

	if (useScreenRefreshRate)
	{
		desiredFrametime = 1 / (mode->refreshRate + 1);
	}
	while (!glfwWindowShouldClose(window))
	{
		vec4 col = mainCamera->clearColour;
		glClearColor(col.r, col.g, col.b, col.a);
		UpdateWindowSize(window);
		KTYGlobal::windowSize = EngineInfo.KTYGlobal::windowSize;
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
			glViewport(0, 0, EngineInfo.KTYGlobal::windowSize.x, EngineInfo.KTYGlobal::windowSize.y);
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

			if (mainCamera->Position.x > startPos.x + 100 ||
				mainCamera->Position.y > startPos.y + 100 ||
				mainCamera->Position.z > startPos.z + 100 ||

				mainCamera->Position.x < startPos.x - 100 ||
				mainCamera->Position.y < startPos.y - 100 ||
				mainCamera->Position.z < startPos.z - 100 ||
				pad.padState.buttons[GLFW_GAMEPAD_BUTTON_X]
				)
			{
				mainCamera->Position = startPos;
				mainCamera->pitch = 4.100014f;
				mainCamera->yaw = 4.899979f;
				mainCamera->SetCameraDir();
			}
			if (pad.padState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] && !r3Pressed) {
				EngineInfo.drawGizmos = !EngineInfo.drawGizmos;
				drawGizmos = EngineInfo.drawGizmos;
				r3Pressed = true;
				Console.Log(EngineInfo.drawGizmos);
			}
			else if (!pad.padState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] && r3Pressed) {
				r3Pressed = false;
			}

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

				if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || pad.padState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) && mouseClickedThisFrame == false)
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
				else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE || !pad.padState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) && mouseClickedThisFrame)
				{
					mouseClickedThisFrame = false;

				}
				if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || pad.padState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]))
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
			Console.Log(std::to_string(frameRate));
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
			//objects[i]->Destroy;
		}
		catch (std::exception e) {}
	}
	objects.clear();
	return 0;
}*/
#pragma endregion SeaOfDeadCode