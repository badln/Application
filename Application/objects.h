
#include <iostream>

#include "GLFW\Include\glad\glad.h"
#include "GLFW\Include\glfw3.h"

#include "GLFW/Include/glm/glm.hpp"
#include "GLFW/Include/glm/gtc/matrix_transform.hpp"
#include "GLFW/Include/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

extern int generatedTexture;
extern int frame;
extern vector <unsigned int> VAOs;

void SetWindowIcon(const char* path, GLFWwindow* window);
class Camera {
public:

	vec3 Position = vec3(0.0f, 0.0f, 3.0f);

	vec3 Target = vec3(0);

	vec3 Direction = normalize(Position - Target);

	float SpeedMultiplier = 3;
	vec3 zero = vec3(0.0f);
	vec3 one = vec3(1.0f);

	vec3 forward = vec3(0.0f, 0.0f, -1.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = vec3(1.0f, 0.0f, 0.0f);
	vec3 lookDirection;
	float fov = 90;

};

struct {
	const int Point	      { 0 };  //LightType.Point	      (is equal to 0)
	const int Directional { 1 };  //LightType.Directional (is equal to 1)
	const int Spot	      { 2 };  //LightType.Spot        (is equal to 2)
	const int Ambient	  { 3 };  //LightType.Ambient     (is equal to 3)
} LightType ;

class LightSource
{
public:
	int renderQueue = 0;
	bool enabled = false;
	float linear = 0.07f;
	float quadratic = 0.017f;
	float constant = 1.0f;
	float cutoff = 1.5f;
	float outerCutoff = 17.5f;
	int type = 0;
	vec3 direction;

	vec4 colour = vec4(1.0f);
	vec3 position = vec3(0.0f);
	vec3 ambient = vec3(1.0f);
	vec3 diffuse = vec3(1.0f);
	vec3 specular = vec3(1.0f);
	
	LightSource(int lightType, vec3 lightDirection);
	LightSource();
};
extern int pointLightNum;
extern int spotLightNum;
struct {
	const GLenum Nearest =		 GL_NEAREST_MIPMAP_NEAREST;
	const GLenum NearestLinear = GL_NEAREST_MIPMAP_LINEAR;
	const GLenum Linear =	     GL_LINEAR_MIPMAP_LINEAR;
	const GLenum LinearNearest = GL_LINEAR_MIPMAP_NEAREST;
} TextureFilter ;
class Texture
{
	bool texAssigned_ = false;
	bool idAssigned_ = false;
	bool manualFiltering_ = false;
	unsigned int data_;
	int id_;
	bool error_ = false;
	GLenum filterMode_ = TextureFilter.Linear;
	GLenum magFilterMode_ = GL_LINEAR;
	GLenum wrapMode_ = GL_REPEAT;
public:
	const unsigned int& data() const { return data_; }
	const bool& texAssigned() const { return texAssigned_; }
	const int& id() const { return id_; }
	const bool& error() const { return error_; }
	const bool& manualFiltering() const{ return manualFiltering_; }
	const GLenum filterMode() const { return filterMode_; }
	const GLenum wrapMode() const { return filterMode_; }
	const char* path; 

	Texture(const char* location, int size = 1);
	Texture();
	void wrapMode(GLenum mode);
	void filterMode(GLenum mode, bool update = false);

	vec4 colour = vec4(1.0f);


	void Set(const char* location, int size = 1);
	void Set();
private:
	void GenErrorTex(unsigned char* ErrorTex, int texture);
};

class Shader
{
	string name_;
public:
	const string& name() const { return name_; }
	unsigned int ID;
	Shader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH, string shaderName);

	void use();
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
	void setVector(const string& name, float valueX, float valueY, float valueZ, float valueW = 1) const;
	void setMatrix(const string& name, mat4 matrix);
	void setMaterial(const string& name, vec3 diffuse, vec3 specular, vec3 ambient, float shininess, vec4 colour, vec4 texColour, int texId, bool texAssigned, int difTexId, bool difTexAssigned, int specTexId, bool specTexAssigned, bool texError, bool difTexError, bool specTexError, float emmissive, int emmissiveTexId, bool emmissiveTexAssigned, bool emmissiveError);
	void setLight(LightSource& light, vec3 parentObjPos);
private:

	void checkCompileErrors(unsigned int shader, string type);
};
extern vector <Texture*> textures;

struct {
	int Back = 0;
	int Front = 1;
	int BackAndFront = 2;
	int None = 3;
} FaceCulling ;
class Material
{
public:
	int culling = FaceCulling.Back;
	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(1.0f);
	Texture diffuseTex;
	vec3 specular = vec3(0.5f);
	Texture specularTex;
	float emmissive;
	Texture emmissiveTex;
	float shininess = 100.0f;
	vec4 colour = vec4(1.0f);
	Texture texture;
	Shader *shader;
};


class Transform {
public:
	vec3 position	   = vec3(0);
	vec3 localPosition = vec3(0);
	vec3 rotation	   = vec3(0);
	vec3 scale         = vec3(1);
};

class Mesh {
	unsigned int VertArrayObj;
public:
	const unsigned int& VAO() const { return VertArrayObj; }
	struct {
		vector <vec3> positions;
		vector <vec3> colours;
		vector <vec3> normals;
		vector <vec2> texCoords;
	} vertexData;
	void Set(float* vertData, int vertDataSize);
};

class Renderer {
public:
	Material material = Material();
	Mesh mesh = Mesh();
};

class ObjContainer {
	string name_ = "Object";
public:
	ObjContainer(const char* objectName);
	ObjContainer();
	void name(const char* string);
	const string& name() const { return name_; }
	Transform transform = Transform();
	Renderer renderer = Renderer();
	LightSource light; 
};
extern vector <ObjContainer*> objects;
