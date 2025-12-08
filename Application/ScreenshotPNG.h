#pragma once
#include "main.h"
#include <filesystem>
#include "global.h"
#include <iostream>
#include <fstream>

class Screenshot {
	static bool take;
	static std::string output;
public:
	/// <summary>
	/// Takes a PNG screenshot in the RGBA8 format for transparency. By default stored in "C:/Users/%USER%/Pictures/KittyEngine/".
	/// Will not take the screenshot instantly, but will queue a screenshot to be taken after all rendering is complete. 
	/// </summary>
	/// <param name="_output"></param>
	static void TakeScreenshot(std::string _output = "null");

	/// <summary>
	/// Takes a PNG screenshot in the RGBA8 format instantly. It is reccomended to just use Screenshot::TakeScreenshot(), though.
	/// </summary>
	static void PNG_RGBA8();
};