#pragma once

#include "main.h"

extern std::vector <Camera*> worldCameras;
vec4 ParseVector(std::string vector);
std::string parameter(std::string str);
std::string thisLineValue(std::string scene);
bool str_bool(std::string string);
int ParseStringToInt(std::string string);
float ParseStringToFloat(std::string string);
void ReadScene(std::string scenePath);
void SetActiveScene(std::string scenePath);