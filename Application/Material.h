#pragma once
#include "main.h"
#include "Texture.h"
#include "Shader.h"
#include "DefaultShaders.h"

class Material {
public:
	float shininess = 256;
	Shader* shader = &DefaultShaders::Lit;
	Texture diffuse;
	Texture specular;
	Texture emissive;
	Texture normal;

	float alpha = 1;
	Colour ambient         = Colour(1);
	Colour diffuseColour   = Colour(1);
	Colour specularColour  = Colour(0.5);
	Colour emissiveColour  = Colour(0);
	Colour normalColour    = Colour(1);
};