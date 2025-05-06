#pragma once
#include <iostream>
#include "Libraries\Include\glfw3.h"

struct {

	std::string EngineName =	"8093 Engine";
	std::string EngineVer =		"0.0.2";
	std::string EngineAuthor =  "Faye Kendall";
	float desiredFramerate = 200;
	float pi = 3.1415926535;
	GLenum defaultFiltering = GL_NEAREST_MIPMAP_NEAREST;

	std::string defaultShaderLoc = "Shaders";
	std::string defaultObjectLoc = "Objects";
	std::string defaultTexLoc =    "Images";

	float MouseSensitivity = 0.1f;
	glm::vec2 windowSize = glm::vec2(1200, 800);
	glm::vec2 MousePosLF = glm::vec2(0);
	glm::vec2 MousePos =   glm::vec2(0);

	void LogEngineInfo()
	{
		std::cout << EngineName << " Version " << EngineVer << ", Written by " << EngineAuthor << std::endl;
	}
} EngineInfo ;