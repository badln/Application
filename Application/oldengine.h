#ifndef ENGINESETTINGS
#define ENGINESETTINGS

#include <iostream>
#include "main.h"

#define KTY_FULLSCREEN = 0
#define KTY_WINDOWED   = 1
#define KTY_BORDERLESS = 2

static struct {

	std::string EngineName =	"KittyEngine";
	std::string EngineVer =		"0.0.2";
	std::string EngineAuthor =  "Faye Kendall";
	std::string BuildDate = "25/10/2025 D/M/Y";
	float desiredFramerate = 60;
	double pi = 3.1415926535;
	bool drawGizmos = false;
	GLenum defaultFiltering = GL_NEAREST_MIPMAP_NEAREST;

	std::string defaultShaderLoc = "Shaders";
	std::string defaultObjectLoc = "Objects";
	std::string defaultTexLoc =    "Images";

	float MouseSensitivity = 0.1f;
	glm::vec2 windowSize = glm::vec2(1280, 800);
	glm::vec2 renderResolution = glm::vec2(1280, 800);
	glm::vec2 MousePosLF = glm::vec2(0);
	glm::vec2 MousePos =   glm::vec2(0);

	const int max_textures = 6; 

	int currentWindowType = 1;

	void LogEngineInfo()
	{
		std::cout << EngineName << " Version " << EngineVer << " Built " << BuildDate << std::endl;
	}
} EngineInfo ;

#endif