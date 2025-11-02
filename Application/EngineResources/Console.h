/*#include <iostream>



class WindowConsole {
public:
	void Log(std::string str, bool newLine = true, int col = 37)
	{
		std::string newStr = "\033[" + std::to_string(col) + "m" + str + "\033[37m";
		switch (newLine)
		{
		case true:
			std::cout << newStr << std::endl;
			break;
		case false:
			std::cout << newStr;
		}
	}
	void Log(vec2 vec, bool newLine = true, int col = 37)
	{
		Log(std::to_string(vec.x) + ", " + std::to_string(vec.y));
	}
	void Log(vec3 vec, bool newLine = true, int col = 37)
	{
		Log(std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z));
	}
	void Log(vec4 vec, bool newLine = true, int col = 37)
	{
		Log(std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w));
	}
	void Log(int i, bool newLine = true, int col = 37)
	{
		Log(std::to_string(i), newLine, col);
	}
	void Log(float f, bool newLine = true, int col = 37)
	{
		Log(std::to_string(f), newLine, col);
	}
	void PushError(std::string str, bool newLine = true)
	{
		Log(str, newLine, 31);
	}
	void PushError(std::exception e, bool newLine = false)
	{
		PushError("Error!");
		throw e;
	}
	void PushWarning(std::string str, bool newLine = true)
	{
		Log(str, newLine, 35);
	}
};*/