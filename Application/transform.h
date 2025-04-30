//Some specific OpenGL stuff
#include "Libraries\Include\glad\glad.h"

//general stuff
#include <iostream>
#include <string>

//OpenGL Mathematics
#include "Libraries/Include/glm/glm.hpp"
#include "Libraries/Include/glm/gtc/matrix_transform.hpp"
#include "Libraries/Include/glm/gtc/type_ptr.hpp"


using namespace glm;


class Transforms
{
public:
	std::string VecToString(vec3 vec)
	{
		return (std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z));
	}

	vec4 TranslateVector(vec3 initial, vec3 dist)
	{
		vec4 vec(initial, 1.0f);
		mat4 trans = mat4(1.0f);

		trans = translate(trans, dist);
		vec = trans * vec;
		return vec;
	}
	vec4 RotateVector(vec3 initial, float degrees, vec3 axis)
	{
		vec4 vec(initial, 1.0f);
		mat4 trans = mat4(1.0f);

		trans = rotate(trans, radians(degrees), axis);
		vec = trans * vec;
		return vec;
	}

	vec4 ScaleVector(vec3 initial, vec3 scalevec)
	{

		vec4 vec(initial, 1.0f);
		mat4 trans = mat4(1.0f);

		trans = scale(trans, scalevec);
		vec = trans * vec;
		return vec;
	}

	//For matrix stuff

	mat4 TranslateMatrix(mat4 initial, vec3 dist)
	{
		mat4 trans = mat4(1.0f);

		trans = translate(trans, dist);
		trans = trans * initial;
		return trans;
	}
	mat4 RotateMatrix(mat4 initial, float degrees, vec3 axis)
	{
		mat4 trans = mat4(1.0f);

		trans = rotate(trans, radians(degrees), axis);
		trans = trans * initial;
		return trans;
	}
	mat4 ScaleMatrix(mat4 initial, vec3 scaleVec)
	{
		mat4 trans = mat4(1.0f);

		trans = scale(trans, scaleVec);
		trans = trans * initial;
		return trans;
	}

};
