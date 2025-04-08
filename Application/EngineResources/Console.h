#include <iostream>

using namespace std;

class WindowConsole {
public:
	void PushLine(string str, bool newLine = true, int col = 37)
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
	void PushLine(vec2 vec, bool newLine = true, int col = 37)
	{
		PushLine(to_string(vec.x) + ", " + to_string(vec.y));
	}
	void PushLine(vec3 vec, bool newLine = true, int col = 37)
	{
		PushLine(to_string(vec.x) + ", " + to_string(vec.y) + ", " + to_string(vec.z));
	}
	void PushLine(vec4 vec, bool newLine = true, int col = 37)
	{
		PushLine(to_string(vec.x) + ", " + to_string(vec.y) + ", " + to_string(vec.z) + ", " + to_string(vec.w));
	}
	void PushError(string str, bool newLine = true)
	{
		PushLine(str, newLine, 31);
	}
	void PushError(exception e, bool newLine = false)
	{
		PushError("Error!");
		throw e;
	}
	void PushWarning(string str, bool newLine = true)
	{
		PushLine(str, newLine, 35);
	}
};