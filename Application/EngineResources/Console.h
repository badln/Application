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
	void PushError(string str, bool newLine = true)
	{
		PushLine(str, newLine, 31);
	}
	void PushWarning(string str, bool newLine = true)
	{
		PushLine(str, newLine, 35);
	}
};