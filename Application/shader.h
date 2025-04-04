#ifndef SHADER_H
#define SHADER_H

#include "GLFW\Include\glad\glad.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "surfaces.h"

#include "GLFW/Include/glm/glm.hpp"
#include "GLFW/Include/glm/gtc/matrix_transform.hpp"
#include "GLFW/Include/glm/gtc/type_ptr.hpp"

//---------------------------------------------
//there was, in fact, a better way of doing it.
//---------------------------------------------

using namespace std;
using namespace glm;

class Shader
{
public:

	unsigned int ID;
	Shader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH)
	{
		bool vertexNull = false;
		bool fragmentNull = false;
		if (VERTEX_SHADER_PATH == "null")
			vertexNull = true;
		if (FRAGMENT_SHADER_PATH == "null")
			fragmentNull = true;
		string VERTEX_SHADER_CODE;
		string FRAGMENT_SHADER_CODE;
		ifstream VERTEX_SHADER_FILE;
		ifstream FRAGMENT_SHADER_FILE;

		VERTEX_SHADER_FILE.exceptions(ifstream::badbit || ifstream::failbit);
		FRAGMENT_SHADER_FILE.exceptions(ifstream::badbit || ifstream::failbit);
		try
		{
			if (!vertexNull)
			{
				VERTEX_SHADER_FILE.open(VERTEX_SHADER_PATH);
				stringstream VERTEX_SHADER_STREAM;
				VERTEX_SHADER_STREAM << VERTEX_SHADER_FILE.rdbuf();
				VERTEX_SHADER_FILE.close();
				VERTEX_SHADER_CODE = VERTEX_SHADER_STREAM.str();
			}

			if (!fragmentNull)
			{
				FRAGMENT_SHADER_FILE.open(FRAGMENT_SHADER_PATH);
				stringstream FRAGMENT_SHADER_STREAM;
				FRAGMENT_SHADER_FILE.open(FRAGMENT_SHADER_PATH);
				FRAGMENT_SHADER_STREAM << FRAGMENT_SHADER_FILE.rdbuf();
				FRAGMENT_SHADER_FILE.close();
				FRAGMENT_SHADER_CODE = FRAGMENT_SHADER_STREAM.str();
			}
		}
		catch (ifstream::failure)
		{
			std::cout << "Shader file(s) not read correctly!"  << "\n";
		}
		const char* VSHADER_SOURCE = VERTEX_SHADER_CODE.c_str();
		const char* FSHADER_SOURCE = FRAGMENT_SHADER_CODE.c_str();

		unsigned int VERTEX_SHADER, FRAGMENT_SHADER;
		//Vertex Shader

		VERTEX_SHADER = glCreateShader(GL_VERTEX_SHADER);
		FRAGMENT_SHADER = glCreateShader(GL_FRAGMENT_SHADER);

		if (!vertexNull)
		{
			glShaderSource(VERTEX_SHADER, 1, &VSHADER_SOURCE, NULL);
			glCompileShader(VERTEX_SHADER);

			checkCompileErrors(VERTEX_SHADER, "VERTEX_SHADER");
		}

		//Fragment Shader

		if (!fragmentNull)
		{
			glShaderSource(FRAGMENT_SHADER, 1, &FSHADER_SOURCE, NULL);
			glCompileShader(FRAGMENT_SHADER);

			checkCompileErrors(FRAGMENT_SHADER, "FRAGMENT_SHADER");
		}

		//Shader Program
		ID = glCreateProgram();
		if (!vertexNull)
			glAttachShader(ID, VERTEX_SHADER);

		if (!fragmentNull)
			glAttachShader(ID, FRAGMENT_SHADER);
		glLinkProgram(ID);

		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(VERTEX_SHADER);
		glDeleteShader(FRAGMENT_SHADER);

		
	}

	void use()
	{
		glUseProgram(ID);
	}

	void setBool(const string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVector(const string& name, float valueX, float valueY, float valueZ, float valueW = 1) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), valueX, valueY, valueZ, valueW);
	}
	void setMatrix(const string& name, glm::mat4 matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void setMaterial(const string& name, Material material)
	{
		setFloat(name + ".diffuse",   material.diffuse);
		setFloat(name + ".specular",  material.specular);
		setFloat(name + ".ambient",   material.ambient);
		setFloat(name + ".shininess", material.shininess);
		setVector(name + ".colour",   material.colour.x,
									  material.colour.y, 
									  material.colour.z, 
									  material.colour.w);
	}
private:

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

#endif