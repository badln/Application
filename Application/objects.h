
#include <iostream>

#include "Libraries/Include/glad/glad.h"
#include "Libraries/Include/glfw3.h"

#include "Libraries/Include/glm/glm.hpp"
#include "Libraries/Include/glm/gtc/matrix_transform.hpp"
#include "Libraries/Include/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include "engine.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;

extern int frame;
extern unsigned int lightPointArray;
extern vec2 windowSize;
void SetWindowIcon(const char* path, GLFWwindow* window);
class Camera {
private:
	std::string name_;
public:

	bool firstCamFrame = false;

	void SetCameraDir();
	void SetCameraDir(GLFWwindow* window, double xposIn, double yposIn, bool looking);
	void SetMainCamera();
	std::string name();
	void name(std::string str);

	float yaw = -90;
	float pitch = 0;

	vec4 viewPort = vec4(0, 0, EngineInfo.renderResolution.x, EngineInfo.renderResolution.y);
	float nearClipPlane = 0.1f;
	float farClipPlane = 200.0f;
	vec4 clearColour = vec4(0.1f, 0.1f, 0.1f, 0.1f);

	vec3 Position = vec3(0.0f, 0.0f, 3.0f);
	vec3 Target = vec3(0);
	vec3 Direction = normalize(Position - Target);

	vec3 zero = vec3(0.0f);
	vec3 one = vec3(1.0f);

	vec3 forward = vec3(0.0f, 0.0f, -1.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = vec3(1.0f, 0.0f, 0.0f);
	vec3 lookDirection;
	float fov = 90;

};
extern Camera* mainCamera;
typedef enum {
	Point	     = 0,   //LightType.Point
	Directional  = 1,   //LightType.Directional
	Spot	     = 2,   //LightType.Spot
	Ambient	     = 3    //LightType.Ambient
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
typedef enum {
	Nearest        = GL_NEAREST_MIPMAP_NEAREST,
	NearestLinear  = GL_NEAREST_MIPMAP_LINEAR,
	Linear         = GL_LINEAR_MIPMAP_LINEAR,
	LinearNearest  = GL_LINEAR_MIPMAP_NEAREST
} TextureFilter ;
typedef enum {
	Texture2D = 0,
	Diffuse   = 1,
	Specular  = 2,
	Emissive  = 3,
	Render    = 4
} TextureType ;
class Texture
{ 
	bool texAssigned_ = false;
	bool idAssigned_ = false;
	bool manualFiltering_ = false;
	unsigned int data_;
	unsigned int id_;
	bool error_ = false;
	GLenum filterMode_ = TextureFilter::Linear;
	GLenum magFilterMode_ = GL_LINEAR;
	GLenum wrapMode_ = GL_REPEAT;
	vec2 size_;
public:
	int type = TextureType::Diffuse; 
	const vec2& size() const { return size_; }
	const unsigned int& data() const { return data_; }
	const bool& texAssigned() const { return texAssigned_; }
	const unsigned int& id() const { return id_; }
	const bool& error() const { return error_; }
	const bool& manualFiltering() const{ return manualFiltering_; }
	const GLenum filterMode() const { return filterMode_; }
	const GLenum wrapMode() const { return filterMode_; }
	std::string path; 

	Texture(std::string location, int typeNum, vec2 dimensions = vec2(0), bool flip = false, int size = 1);
	Texture(int typeNum, vec2 dimensions = vec2(0), bool flip = false, int size = 1);
	Texture();
	void wrapMode(GLenum mode);
	void filterMode(GLenum mode, bool update = false);

	vec4 colour = vec4(1.0f);


	void Set(std::string location, int typeNum, vec2 dimensions = vec2(0), bool flip = false, int size = 1);
	void Set();
private:
	void GenErrorTex(unsigned char* ErrorTex, int texture);
};
extern std::vector <Texture> globalTextures;
class Transform;
class Shader
{
	std::string name_;
public:
	const std::string& name() const { return name_; }
	unsigned int ID;
	Shader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH, std::string shaderName, const char* GEOMETRY_SHADER_PATH = nullptr);

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVector(const std::string& name, float valueX, float valueY, float valueZ, float valueW = 1) const;
	void setMatrix(const std::string& name, mat4 matrix);
	void setMaterial(const std::string& name, vec3 diffuse, vec3 specular, vec3 ambient, float shininess, vec4 colour, vec4 texColour, int texId, bool texAssigned, int difTexId, bool difTexAssigned, int specTexId, bool specTexAssigned, bool texError, bool difTexError, bool specTexError, float emissive, int emissiveTexId, bool emissiveTexAssigned, bool emissiveError);
	void setLight(LightSource& light, vec3 parentObjPos);
	void setMatAndTransform(Transform trans, mat4 mat);
private:

	void checkCompileErrors(unsigned int shader, std::string type);
};
mat4 SetupMatrix(Transform trans, mat4 mat);
extern std::vector <Shader*> shaders;

struct {
	int Back = 0;
	int Front = 1;
	int BackAndFront = 2;
	int None = 3;
} FaceCulling ;

class Material {
public:
	int culling = FaceCulling.Back;
	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(1.0f);
	Texture diffuseTex;
	vec3 specular = vec3(0.5f);
	Texture specularTex;
	float emissive;
	Texture emissiveTex;
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
	vec3 localRotation = vec3(0);
	vec3 scale         = vec3(1);
	vec3 localScale	   = vec3(1);
};

struct Vertex {
	vec3 Position;
	vec3 Colour;
	vec3 Normal;
	vec2 TexCoord;
} ;
class Mesh {
	unsigned int VAO_, VBO_, EBO_;
	void setupMesh();
public:
	const unsigned int& VAO() const { return VAO_; }
	const unsigned int& VBO() const { return VBO_; }
	const unsigned int& EBO() const { return EBO_; }
	struct {
		std::vector<Vertex>	      vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture>	  textures;
	} data;
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(int faceCulling, Shader &shader, float pi, Transform transform);
};

class ObjContainer;

class Model {
private:
public:
	std::string directory;
	bool flipTex;

	void loadModel(std::string path, bool flipTextures, ObjContainer* obj);
	void processNode(aiNode* node, const aiScene* scene, bool flip, ObjContainer* obj);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, bool flip, ObjContainer* obj);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, int typeNum, bool flip = false);
	Model();
	Model(const char* path, bool flipTextures = false)
	{
		directory = path;
		flipTex = flipTextures;
	}
};


class Renderer {
public:
	Material material = Material();
	Mesh mesh = Mesh();
};
extern ObjContainer Scene;
class ObjContainer {
	std::string name_ = "Object";
	int placeInArray = 0, placeInParentArray = 0;
	bool placed = false;
	ObjContainer* parent_ = this;
	std::vector<ObjContainer*> children;
public:

	void Init();
	bool active = true;

	ObjContainer* GetChild(std::string path);
	ObjContainer* GetChild(int place);
	static ObjContainer* Find(std::string pathToObj);

	void SetParent(ObjContainer* thisParent);
	void SetParent(std::string pathToParent);
	const ObjContainer& parent() const { return *parent_; }

	void SetModel(std::string path, bool flipTextures = false);
	void SetModel(const char* path, bool flipTextures = false);
	void SetModel(Model model, bool flipTextures = false);
	void Draw(Shader &lightGizmo, mat4 projection, mat4 view, vec2 windowSize);

	const int& childCount() const { return children.size(); }
	ObjContainer(const char* objectName);
	ObjContainer();
	void name(const char* string);
	const std::string& name() const { return name_; }
	Transform transform = Transform();
	Renderer renderer = Renderer();
	LightSource light; 
	void Destroy();
};
extern std::vector <ObjContainer*> objects;
extern int drawCalls;

class Framebuffer {
private:
	std::string name_;
	unsigned int FBO_, RBO_;
public:
	Texture* texture;
	void Create(GLenum FBtype = GL_FRAMEBUFFER, std::string name = "New Framebuffer");
	void Create(Texture* tex, GLenum FBtype = GL_FRAMEBUFFER, std::string name = "New Framebuffer");
	const std::string& name() const { return name_; }
	const int& FBO() const { return FBO_; }
	const int& RBO() const { return RBO_; }
	void use(unsigned int framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glActiveTexture(GL_TEXTURE0 + texture->id());
	}
	void use() {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
		glActiveTexture(GL_TEXTURE0 + texture->id());
	}
	void name(std::string name);
	void Delete() {
		glDeleteFramebuffers(1, &FBO_);
	}
};