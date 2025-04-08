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
	vec3 ambient  = vec3(0.1f);
	vec3 diffuse  = vec3(1.0f);
	vec3 specular = vec3(0.5f);
	float radius  = 10.0f;

};
class Material
{
public:
	vec3 ambient    = vec3(0.1f);
	vec3 diffuse    = vec3(1.0f);
	vec3 specular   = vec3(0.5f);
	float shininess = 1.0f;
	vec4 colour = vec4(1.0f);

	bool textureAssigned = false;
	unsigned int texture;
	void SetTexture(unsigned int ID, bool assigned)
	{
		texture = ID;
		textureAssigned = assigned;
	}
};
