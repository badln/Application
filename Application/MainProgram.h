#ifndef MAINWINDOW
#define MAINWINDOW

#include "ProgramBase.h"
#include "KTYEngine.h";
#include "Entity.h"

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
unsigned int VBO, VAO; 
ShaderProgram lit;

class MainProgram : public Program {

	Time::Timer timer;

	void PreInit() override { 
		windowSize = Vector2(800, 800);
	}

	void Start() override {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		lit.CreateShader("vertex.glsl", "fragment.glsl");
		lit.Use();

		glClearColor(0.502, 0.859, 1, 1);
	}
	float xVal = 0, yVal = 0;
	void Update() override {

		glm::mat4 ProjectionMatrix = glm::perspective((float)glm::radians(75.0f), (float)(windowSize.x() / windowSize.y()), 0.1f, 100.0f);

		glm::mat4 ModelTransform = glm::mat4(1.0f);
		if (ActiveKeyboard.Key(key_pressed, key_up)) {
			yVal += Time::deltaTime() * 100;
		}
		if (ActiveKeyboard.Key(key_pressed, key_down)) {
			yVal -= Time::deltaTime() * 100;
		}
		if (ActiveKeyboard.Key(key_pressed, key_left)) {
			xVal -= Time::deltaTime() * 100;
		}
		if (ActiveKeyboard.Key(key_pressed, key_right)) {
			xVal += Time::deltaTime() * 100;
		}


		ModelTransform = glm::translate(ModelTransform, Vector3(0,0,-3).glm());
		ModelTransform = glm::rotate(ModelTransform, (float)(xVal * (KTY_PI / 180)), Vector3(0,1,0).glm());
		ModelTransform = glm::rotate(ModelTransform, (float)(yVal * (KTY_PI / 180)), Vector3(1,0,0).glm());

		Vector3 newCol = Vector3(1, 0, glm::sin(Time::SinceStartup()));

		lit.SetVar<mat4>(ModelTransform, STR(ModelTransform));
		lit.SetVar<mat4>(ProjectionMatrix, STR(ProjectionMatrix));
		lit.SetVar<Vector3>(newCol, STR(newCol));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

#endif