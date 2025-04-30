#include <iostream>
#include "Libraries\Include\glfw3.h"

class EngineInfo
{
public:
	std::string EngineName =	    "8093 Engine";
	std::string EngineVer =		"0.0.2";
	std::string EngineAuthor =   "Faye Kendall";
	float desiredFramerate = 200;
	float pi = 3.1415926535;
	GLenum defaultFiltering = GL_NEAREST_MIPMAP_NEAREST;

	std::string defaultShaderLoc = "Shaders";
	std::string defaultObjectLoc = "Objects";
	std::string defaultTexLoc =    "Images";

	void LogEngineInfo()
	{
		std::cout << EngineName << " Version " << EngineVer << ", Written by " << EngineAuthor << std::endl;
	}
};