#ifndef STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "objects.h"

void SetWindowIcon(const char* path, GLFWwindow* window)
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(path, &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
}
void Texture::Set(std::string location, int typeNum, bool flip, int size)
{

	if (!texAssigned_)
	{
		type = typeNum;
		path = location;
		stbi_set_flip_vertically_on_load(flip);
		unsigned int tex;
		int width, height, nrChannels;
		glGenTextures(size, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		unsigned char* imageData = stbi_load(location.c_str(), &width, &height, &nrChannels, 0);
		if (!imageData)
		{
			unsigned int errorTex{};
			int ErrorW, ErrorH, ErrorNrChannels;
			unsigned char* ErrorTex = stbi_load("EngineResources/ERROR.bmp", &ErrorW, &ErrorH, &ErrorNrChannels, 4);
			GenErrorTex(ErrorTex, errorTex);
		}
		else {

			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode_);
			if (!idAssigned_)
			{
				generatedTexture++;
				id_ = generatedTexture;
				idAssigned_ = true;
				globalTextures.push_back(*this);
			}
			texAssigned_ = true;
		}
		data_ = tex;
		stbi_image_free(imageData);
	}
	else {
		texAssigned_ = false;
		glDeleteTextures(1, &data_);
		Set();
	}
}
Texture::Texture(std::string location, int type, bool flip, int size)
{
	Set(location, type, flip, size);
}
Texture::Texture()
{
	
}
void Texture::Set()
{
	Set(path, type);
}
void Texture::wrapMode(GLenum mode)
{
	wrapMode_ = mode;
	Set();
}
void Texture::filterMode(GLenum mode, bool update)
{
	if (mode == GL_NEAREST_MIPMAP_NEAREST || mode == GL_NEAREST_MIPMAP_LINEAR) {
		magFilterMode_ = GL_NEAREST;
		filterMode_ = mode;
		manualFiltering_ = true;
		if (frame > 0)
		{
			Set();
		}
	}
	else if (mode == GL_LINEAR_MIPMAP_LINEAR || mode == GL_LINEAR_MIPMAP_NEAREST) {
		magFilterMode_ = GL_LINEAR;
		filterMode_ = mode;
		manualFiltering_ = true;
		if (frame > 0)
		{
			Set();
		}

	}
	if (!update)
	{
		globalTextures[id() - 1].filterMode(mode, true);
	}
}

void Texture::GenErrorTex(unsigned char* ErrorTex, int texture)
{
	if (ErrorTex)
	{
		std::cout << "Image incorrectly loaded.\n";
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, ErrorTex);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		texAssigned_ = false;
		error_ = true;
	}
}

LightSource::LightSource() {
	LightSource(0, vec3(0));
}
LightSource::LightSource(int lightType, vec3 lightDirection)
{
	type = lightType;
	direction = lightDirection;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->data.vertices = vertices;
	this->data.indices  = indices;
	this->data.textures = textures;

	setupMesh();
}
void Mesh::setupMesh()
{

	unsigned long long VertexSize = sizeof(Vertex);

	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_); 

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * VertexSize, &data.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), &data.indices[0], GL_STATIC_DRAW);

	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);
	glEnableVertexAttribArray(0);
	//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	//texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
void Mesh::Draw(int faceCulling, Shader &shader, float pi, Transform transform)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int emissiveNr = 0;
	for (unsigned int i = 0; i < data.textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + data.textures[i].id());
		glBindTexture(GL_TEXTURE_2D, data.textures[i].id());
		//std::cout << "Texture " << data.textures[i].id() << " has path " << data.textures[i].path << " and type " << data.textures[i].type << "\n";

		if (data.textures[i].type == TextureType.Diffuse) {
			std::string loc = "diffuseTextures[" + std::to_string(diffuseNr) + "]";
			shader.setInt(loc, data.textures[i].id());
			shader.setBool("diffuseTexturesAs[" + std::to_string(diffuseNr) + "]", true);
			//std::cout << "Set diffuse texture " << data.textures[i].id() << " with path " << data.textures[i].path << " '" + loc + "'\n";
			diffuseNr++;
		}
		else if (data.textures[i].type == TextureType.Specular) {
			std::string loc = "specularTextures[" + std::to_string(specularNr) + "]";
			shader.setInt(loc, data.textures[i].id());
			shader.setBool("specularTexturesAs[" + std::to_string(specularNr) + "]", true);
			//std::cout << "Set specular texture " << data.textures[i].id() << " with path " << data.textures[i].path << " '" + loc + "'\n";
			specularNr++;
		}
		else if (data.textures[i].type == TextureType.Emissive) {
			std::string loc = "emissiveTextures[" + std::to_string(emissiveNr) + "]";
			shader.setInt(loc, data.textures[i].id());
			shader.setBool("emissiveTexturesAs[" + std::to_string(emissiveNr) + "]", true);
			emissiveNr++;
		}
	}
	if (faceCulling == FaceCulling.Back)
		glCullFace(GL_BACK);
	else if (faceCulling == FaceCulling.Front)
		glCullFace(GL_FRONT);
	else if (faceCulling == FaceCulling.BackAndFront)
		glCullFace(GL_FRONT_AND_BACK);
	else if (faceCulling == FaceCulling.None)
		glDisable(GL_CULL_FACE);

	shader.setMatAndTransform(transform, mat4(1), pi);
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (faceCulling == FaceCulling.None)
		glEnable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
}
Mesh::Mesh() {}


void ObjContainer::Draw(int faceCulling, Shader *shader, float pi)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i].active)
		{
			children[i].transform.position = this->transform.position + this->transform.localPosition;
			children[i].transform.rotation = this->transform.rotation + this->transform.localRotation;
			children[i].transform.scale =	 this->transform.scale    * this->transform.localScale;
			children[i].renderer.mesh.Draw(faceCulling, *shader, pi, children[i].transform);
		}
	}
}
Model::Model() {}
void ObjContainer::SetModel(std::string path, bool flipTextures)
{
	Model model(path, this, flipTextures);
}
void ObjContainer::SetModel(Model model, bool flipTextures)
{
	model.loadModel(model.directory, model.flipTex, this);
}
void Model::loadModel(std::string path, bool flipTextures, ObjContainer* obj)
{
	std::ifstream test(path);
	if (!test)
	{
		std::cout << "Path '" << path << "' does not exist!\n";
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR : " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, scene, flipTextures, obj);
}
void Model::processNode(aiNode* node, const aiScene* scene, bool flip, ObjContainer* obj)
{
	static ObjContainer objs[10];
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		const char* mName = mesh->mName.C_Str();
		objs[i].name(mName);
		std::cout << "Creating child object, nameof '" << objs[i].name() << "'\n";
		objs[i].renderer.mesh = processMesh(mesh, scene, flip, obj);
		objs[i].parent = obj;
		objs[i].renderer.material = obj->renderer.material;
		obj->children.push_back(objs[i]);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, flip, obj);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, bool flip, ObjContainer* obj)
{
	std::vector<Vertex>		  vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vert;

		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vert.Position = vector;
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vert.Normal = vector;
		}
		if (mesh->mTextureCoords[0])
		{
			vec2 vector;
			vector.x = mesh->mTextureCoords[0][i].x;
			vector.y = mesh->mTextureCoords[0][i].y;
			vert.TexCoord = vector;
		}
		else {
			vert.TexCoord = vec2(0);
		}
		vertices.push_back(vert);
	}
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int x = 0; x < face.mNumIndices; x++)
		{
			indices.push_back(face.mIndices[x]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector <Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, 1, flip);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector <Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, 2, flip);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector <Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, 3, flip);
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());


	}
	return Mesh(vertices, indices, textures);
}
std::vector <Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, int typeNum, bool flip)
{
	std::vector <Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		
		mat->GetTexture(type, i, &str);
		bool flag = false;
		std::string loc = str.C_Str();
		try {
			loc = loc.substr(loc.find_last_of("/"), loc.substr().length());
		} catch (std::exception e){ }
		if (loc.substr(0, 1) != "/")
		{
			loc = "/" + loc;
		}
		loc = directory + loc;
		for (int x = 0; x < globalTextures.size(); x++)
		{
			if (loc == globalTextures[x].path) {
				flag = true;
				textures.push_back(globalTextures[x]);
				break;
			}
		}
		if (!flag)
		{
			Texture tex(loc, typeNum, flip);
			std::ifstream test(tex.path);
			if (test)
			{
				//std::cout << "Image path of '" << tex.path << "' is valid\n";
				textures.push_back(tex);
			} 
			else
			{
				//std::cout << "Image path of '" << tex.path << "' is NOT valid\n";
			}
		}
	}
	
	return textures;
}
Shader::Shader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH, std::string shaderName, const char* GEOMETRY_SHADER_PATH)
{
	bool geometryNull = false;
	if (GEOMETRY_SHADER_PATH == nullptr)
	{
		GEOMETRY_SHADER_PATH = "";
		geometryNull = true;
	}
	std::string r = ("Shaders/" + (std::string)GEOMETRY_SHADER_PATH);
	GEOMETRY_SHADER_PATH = r.c_str();
	std::string s = ("Shaders/" + (std::string)VERTEX_SHADER_PATH);
	VERTEX_SHADER_PATH = s.c_str();
	std::string t = ("Shaders/" + (std::string)FRAGMENT_SHADER_PATH);
	FRAGMENT_SHADER_PATH = t.c_str();

	name_ = shaderName;
	bool vertexNull = false;
	bool fragmentNull = false;
	if (VERTEX_SHADER_PATH == nullptr)
		vertexNull = true;
	if (FRAGMENT_SHADER_PATH == nullptr)
		fragmentNull = true;
	std::string VERTEX_SHADER_CODE;
	std::string FRAGMENT_SHADER_CODE;
	std::string GEOMETRY_SHADER_CODE;
	std::ifstream VERTEX_SHADER_FILE;
	std::ifstream FRAGMENT_SHADER_FILE;
	std::ifstream GEOMETRY_SHADER_FILE;

	VERTEX_SHADER_FILE  .exceptions(std::ifstream::badbit || std::ifstream::failbit);
	FRAGMENT_SHADER_FILE.exceptions(std::ifstream::badbit || std::ifstream::failbit);
	GEOMETRY_SHADER_FILE.exceptions(std::ifstream::badbit || std::ifstream::failbit);
	try
	{
		if (!vertexNull)
		{
			VERTEX_SHADER_FILE.open(VERTEX_SHADER_PATH);
			std::stringstream VERTEX_SHADER_STREAM;
			VERTEX_SHADER_STREAM << VERTEX_SHADER_FILE.rdbuf();
			VERTEX_SHADER_FILE.close();
			VERTEX_SHADER_CODE = VERTEX_SHADER_STREAM.str();
		}

		if (!fragmentNull)
		{
			FRAGMENT_SHADER_FILE.open(FRAGMENT_SHADER_PATH);
			std::stringstream FRAGMENT_SHADER_STREAM;
			FRAGMENT_SHADER_STREAM << FRAGMENT_SHADER_FILE.rdbuf();
			FRAGMENT_SHADER_FILE.close();
			FRAGMENT_SHADER_CODE = FRAGMENT_SHADER_STREAM.str();
		}
		if (!geometryNull)
		{
			GEOMETRY_SHADER_FILE.open(GEOMETRY_SHADER_PATH);
			std::stringstream GEOMETRY_SHADER_STREAM;
			GEOMETRY_SHADER_STREAM << GEOMETRY_SHADER_FILE.rdbuf();
			GEOMETRY_SHADER_FILE.close();
			GEOMETRY_SHADER_CODE = GEOMETRY_SHADER_STREAM.str();
		}
	}
	catch (std::ifstream::failure)
	{
		std::cout << "Shader file(s) not read correctly!" << "\n";
	}
	const char* VSHADER_SOURCE = VERTEX_SHADER_CODE.c_str();
	const char* FSHADER_SOURCE = FRAGMENT_SHADER_CODE.c_str();
	const char* GSHADER_SOURCE = GEOMETRY_SHADER_CODE.c_str();

	unsigned int VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER;

	VERTEX_SHADER   = glCreateShader(GL_VERTEX_SHADER);
	FRAGMENT_SHADER = glCreateShader(GL_FRAGMENT_SHADER);
	GEOMETRY_SHADER = glCreateShader(GL_GEOMETRY_SHADER);

	if (!vertexNull)
	{
		glShaderSource(VERTEX_SHADER, 1, &VSHADER_SOURCE, NULL);
		glCompileShader(VERTEX_SHADER);

		checkCompileErrors(VERTEX_SHADER, "VERTEX_SHADER");
	}
	if (!fragmentNull)
	{
		glShaderSource(FRAGMENT_SHADER, 1, &FSHADER_SOURCE, NULL);
		glCompileShader(FRAGMENT_SHADER);

		checkCompileErrors(FRAGMENT_SHADER, "FRAGMENT_SHADER");
	}
	if (!geometryNull)
	{
		glShaderSource(GEOMETRY_SHADER, 1, &GSHADER_SOURCE, NULL);
		glCompileShader(GEOMETRY_SHADER);

		checkCompileErrors(GEOMETRY_SHADER, "GEOMETRY_SHADER");
	}
	ID = glCreateProgram();

	if (!geometryNull)
		glAttachShader(ID, GEOMETRY_SHADER);

	if (!vertexNull)
		glAttachShader(ID, VERTEX_SHADER);

	if (!fragmentNull)
		glAttachShader(ID, FRAGMENT_SHADER);

		
	glLinkProgram(ID);

	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(VERTEX_SHADER);
	glDeleteShader(FRAGMENT_SHADER);
	glDeleteShader(GEOMETRY_SHADER);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	try {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	catch (std::exception e) {}
}
void Shader::setInt(const std::string& name, int value) const
{
	try {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	} catch (std::exception e) {}
}
void Shader::setFloat(const std::string& name, float value) const
{
	try {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	} catch (std::exception e) {}
}
void Shader::setVector(const std::string& name, float valueX, float valueY, float valueZ, float valueW) const
{
	try {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), valueX, valueY, valueZ, valueW);
	} catch (std::exception e) {}
}
void Shader::setMatrix(const std::string& name, glm::mat4 matrix)
{
	try {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}catch (std::exception e) {}
	
}
void Shader::setMaterial(const std::string& name, vec3 diffuse, vec3 specular, vec3 ambient, float shininess, vec4 colour, vec4 texColour, int texId, bool texAssigned, int difTexId, bool difTexAssigned, int specTexId, bool specTexAssigned, bool texError, bool difTexError, bool specTexError, float emissive, int emissiveTexId, bool emissiveTexAssigned, bool emissiveError)
{
	try {
		setVector(name + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
		setVector(name + ".specular", specular.x, specular.y, specular.z);
		setVector(name + ".ambient", ambient.x, ambient.y, ambient.z);
		setFloat(name + ".shininess", shininess);
		setVector(name + ".colour", colour.x * texColour.x,
			colour.y * texColour.y,
			colour.z * texColour.z,
			colour.w * texColour.w);
		setInt(name + ".texture", texId);
		setBool(name + ".as_tex", texAssigned);
		setInt(name + ".diffuseTex", difTexId);
		setBool(name + ".dif_tex", difTexAssigned);
		setInt(name + ".specularTex", specTexId);
		setBool(name + ".spec_tex", specTexAssigned);
		setInt(name + ".emissiveTex", emissiveTexId);
		setBool(name + ".em_tex", emissiveTexAssigned);
		setFloat(name + ".emissive", emissive);

		if (texError || difTexError || specTexError || emissiveError)
		{
			setBool(name + ".texError", true);
		}
	}
	catch (std::exception e) {}
}
void Shader::setMatAndTransform(Transform trans, mat4 mat, float pi)
{
	mat = translate (mat, trans.position + (trans.localPosition * trans.scale));
	mat = rotate	(mat, trans.rotation.x + trans.localRotation.x * (pi / 180), vec3(1.0f, 0.0f, 0.0f));
	mat = rotate	(mat, trans.rotation.y + trans.localRotation.y * (pi / 180), vec3(0.0f, 1.0f, 0.0f));
	mat = rotate	(mat, trans.rotation.z + trans.localRotation.z * (pi / 180), vec3(0.0f, 0.0f, 1.0f));
	mat = scale		(mat, trans.scale * trans.localScale);
	//std::cout << name() << "\n";
	setMatrix("model", mat);
}
void Shader::setLight(LightSource& light, vec3 parentObjPos)
{
	try {
		// 0 = point
		// 1 = direction
		// 2 = spot
		// 3 = ambient
		std::string name;
		switch (light.type)
		{
		case 0:
			name = "pLight";
			if (light.renderQueue == 0)
			{
				light.renderQueue = pointLightNum;
				pointLightNum++;
			}
			setVector(name + "[" + std::to_string(light.renderQueue) + "].diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			setVector(name + "[" + std::to_string(light.renderQueue) + "].specular", light.specular.x, light.specular.y, light.specular.z);
			setVector(name + "[" + std::to_string(light.renderQueue) + "].ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			setVector(name + "[" + std::to_string(light.renderQueue) + "].colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			setVector(name + "[" + std::to_string(light.renderQueue) + "].position", light.position.x + parentObjPos.x, light.position.y + parentObjPos.y, light.position.z + parentObjPos.z);
			setFloat(name +  "[" + std::to_string(light.renderQueue) + "].linear", light.linear);
			setFloat(name +  "[" + std::to_string(light.renderQueue) + "].quadratic", light.quadratic);
			setFloat(name +  "[" + std::to_string(light.renderQueue) + "].constant", light.constant);
			break;
		case 1:
			name = "dirLight";
			setVector(name + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			setVector(name + ".specular", light.specular.x, light.specular.y, light.specular.z);
			setVector(name + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			setVector(name + ".colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			setVector(name + ".direction", light.direction.x, light.direction.y, light.direction.z);
			break;
		case 2:
			name = "sLight";
			if (light.renderQueue == 0)
			{ 
				light.renderQueue = spotLightNum;
				spotLightNum++;
			}
			setFloat(name +  "[" + std::to_string(light.renderQueue - 1) + "].linear", light.linear);
			setFloat(name +  "[" + std::to_string(light.renderQueue - 1) + "].quadratic", light.quadratic);
			setFloat(name +  "[" + std::to_string(light.renderQueue - 1) + "].constant", light.constant);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].specular", light.specular.x, light.specular.y, light.specular.z);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].position", light.position.x + parentObjPos.x, light.position.y + parentObjPos.y, light.position.z + parentObjPos.z);
			setVector(name + "[" + std::to_string(light.renderQueue - 1) + "].direction", light.direction.x, light.direction.y, light.direction.z);
			setFloat(name +  "[" + std::to_string(light.renderQueue - 1) + "].cutoff", cos(radians(light.cutoff)));
			setFloat(name +  "[" + std::to_string(light.renderQueue - 1) + "].outerCutoff", cos(radians(light.outerCutoff)));
			break;
		case 3:
			name = "aLight";
			setVector(name + ".colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			glClearColor(light.colour.x / 15 * light.colour.w, light.colour.y / 15 * light.colour.w, light.colour.z / 15 * light.colour.w, 1);
			break;
		}

	}
	catch (std::exception e) {}

}
void Shader::checkCompileErrors(unsigned int shader, std::string type)
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

ObjContainer::ObjContainer(const char* objectName)
{
	name(objectName);
	objects.push_back(this);
}
ObjContainer::ObjContainer()
{
	name_ = "New Object";
	objects.push_back(this);
}
void ObjContainer::name(const char* string)
{
	name_ = string;

}
ObjContainer* ObjContainer::FindChild(std::string name)
{
	for (int i = 0; i < children.size(); i++) {
		if (children[i].name() == name)
			return &children[i];
	}
	return this;
}
#endif