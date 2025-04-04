#include <iostream>

#include "GLFW/Include/glm/glm.hpp"
#include "GLFW/Include/glm/gtc/matrix_transform.hpp"
#include "GLFW/Include/glm/gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

class LightSource
{
public:

	int type      = 0;
	vec4 colour   = vec4(1.0f);
	vec3 position = vec3(0.0f);
	vec3 rotation = vec3(0.0f);
	float radius  = 10.0f;

};
class Material
{
public:
	float ambient   = 0.1f;
	float diffuse   = 1.0f;
	float specular  = 0.5f;
	float shininess = 1.0f;
	vec4 colour = vec4(1.0f);
};
