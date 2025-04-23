#include <iostream>

using namespace std;

class WindowConsole {
public:
	void Log(string str, bool newLine = true, int col = 37)
	{
		string newStr = "\033[" + to_string(col) + "m" + str + "\033[37m";
		switch (newLine)
		{
		case true:
			cout << newStr << endl;
			break;
		case false:
			cout << newStr;
		}
	}
	void Log(vec2 vec, bool newLine = true, int col = 37)
	{
		Log(to_string(vec.x) + ", " + to_string(vec.y));
	}
	void Log(vec3 vec, bool newLine = true, int col = 37)
	{
		Log(to_string(vec.x) + ", " + to_string(vec.y) + ", " + to_string(vec.z));
	}
	void Log(vec4 vec, bool newLine = true, int col = 37)
	{
		Log(to_string(vec.x) + ", " + to_string(vec.y) + ", " + to_string(vec.z) + ", " + to_string(vec.w));
	}
	void Log(int i, bool newLine = true, int col = 37)
	{
		Log(to_string(i), newLine, col);
	}
	void Log(float f, bool newLine = true, int col = 37)
	{
		Log(to_string(f), newLine, col);
	}
	void PushError(string str, bool newLine = true)
	{
		Log(str, newLine, 31);
	}
	void PushError(exception e, bool newLine = false)
	{
		PushError("Error!");
		throw e;
	}
	void PushWarning(string str, bool newLine = true)
	{
		Log(str, newLine, 35);
	}
};