#include <iostream>
#include "GLFW\Include\glfw3.h"
using namespace std;

class EngineInfo
{
public:
	string EngineName =	    "Graphics Engine";
	string EngineVer =		"0.0.1";
	string EngineAuthor =   "Faye Kendall";
	float desiredFramerate = 1000;
	float pi = 3.1415926535;
	GLenum defaultFiltering = GL_NEAREST_MIPMAP_NEAREST;
	void LogEngineInfo()
	{
		cout << EngineName << " Version " << EngineVer << ", Written by " << EngineAuthor << endl;
	}
};