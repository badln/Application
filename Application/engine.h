#include <iostream>
using namespace std;

class EngineInfo
{
public:
	string EngineName =	    "Graphics Engine";
	string EngineVer =		"0.0.1";
	string EngineAuthor =   "Faye Kendall";
	float desiredFramerate = 60;

	float pi = 3.1415926535;

	void LogEngineInfo()
	{

		cout << EngineName << " Version " << EngineVer << ", Written by " << EngineAuthor << endl;
	}
};