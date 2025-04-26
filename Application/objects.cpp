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
void Texture::Set(const char* location, int size)
{
	if (!texAssigned_)
	{

		path = location;
		stbi_set_flip_vertically_on_load(true);
		unsigned int tex;
		int width, height, nrChannels;
		glGenTextures(size, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		unsigned char* imageData = stbi_load(location, &width, &height, &nrChannels, 0);
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
				textures.push_back(this);
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
Texture::Texture(const char* location, int size)
{
	Set(location, size);
}
Texture::Texture()
{
	
}
void Texture::Set()
{
	Set(path);
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
		textures[id()]->filterMode(mode, true);
	}
}

void Texture::GenErrorTex(unsigned char* ErrorTex, int texture)
{
	if (ErrorTex)
	{
		cout << "Image incorrectly loaded.\n";
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

void Mesh::Set(float* vertData, int vertDataSize)
{
	for (int i = 0; i < ((vertDataSize / sizeof(float)) / 8); i++)
	{

		vec3 pos;
		pos.x = vertData[(-8 + ((i + 1) * 8))];
		pos.y = vertData[(-7 + ((i + 1) * 8))];
		pos.z = vertData[(-6 + ((i + 1) * 8))];
		vertexData.positions.push_back(pos);

		vec3 norm;
		norm.x = vertData[(-5 + ((i + 1) * 8))];
		norm.y = vertData[(-4 + ((i + 1) * 8))];
		norm.z = vertData[(-3 + ((i + 1) * 8))];
		vertexData.normals.push_back(norm);

		vec2 tex;
		tex.x = vertData[(-2 + ((i + 1) * 8))];
		tex.y = vertData[(-1 + ((i + 1) * 8))];
		vertexData.texCoords.push_back(tex);

		vec3 col;
		col = vec3(1.0f);
		vertexData.colours.push_back(col);
	}
	glGenVertexArrays(1, &VertArrayObj);
	glBufferData(GL_ARRAY_BUFFER, vertDataSize, vertData, GL_STATIC_DRAW);
	glBindVertexArray(VertArrayObj);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	VAOs.push_back(VertArrayObj);
}

Shader::Shader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH, string shaderName)
{
	name_ = shaderName;

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
		std::cout << "Shader file(s) not read correctly!" << "\n";
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

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
	try {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	catch (exception e) {}
}
void Shader::setInt(const string& name, int value) const
{
	try {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	} catch (exception e) {}
}
void Shader::setFloat(const string& name, float value) const
{
	try {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	} catch (exception e) {}
}
void Shader::setVector(const string& name, float valueX, float valueY, float valueZ, float valueW) const
{
	try {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), valueX, valueY, valueZ, valueW);
	} catch (exception e) {}
}
void Shader::setMatrix(const string& name, glm::mat4 matrix)
{
	try {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}catch (exception e) {}
	
}
void Shader::setMaterial(const string& name, vec3 diffuse, vec3 specular, vec3 ambient, float shininess, vec4 colour, vec4 texColour, int texId, bool texAssigned, int difTexId, bool difTexAssigned, int specTexId, bool specTexAssigned, bool texError, bool difTexError, bool specTexError, float emmissive, int emmissiveTexId, bool emmissiveTexAssigned, bool emmissiveError)
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
		setInt(name + ".emmissiveTex", emmissiveTexId);
		setBool(name + ".em_tex", emmissiveTexAssigned);
		setFloat(name + ".emmissive", emmissive);

		if (texError || difTexError || specTexError || emmissiveError)
		{
			setBool(name + ".texError", true);
		}
	}
	catch (exception e) {}
}
void Shader::setLight(LightSource& light, vec3 parentObjPos)
{
	try {
		// 0 = point
		// 1 = direction
		// 2 = spot
		// 3 = ambient
		string name;
		switch (light.type)
		{
		case 0:
			name = "pLight";
			if (light.renderQueue == 0)
			{
				light.renderQueue = pointLightNum;
				pointLightNum++;
			}
			setVector(name + "[" + to_string(light.renderQueue) + "].diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			setVector(name + "[" + to_string(light.renderQueue) + "].specular", light.specular.x, light.specular.y, light.specular.z);
			setVector(name + "[" + to_string(light.renderQueue) + "].ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			setVector(name + "[" + to_string(light.renderQueue) + "].colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			setVector(name + "[" + to_string(light.renderQueue) + "].position", light.position.x + parentObjPos.x, light.position.y + parentObjPos.y, light.position.z + parentObjPos.z);
			setFloat(name +  "[" + to_string(light.renderQueue) + "].linear", light.linear);
			setFloat(name +  "[" + to_string(light.renderQueue) + "].quadratic", light.quadratic);
			setFloat(name +  "[" + to_string(light.renderQueue) + "].constant", light.constant);
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
			setFloat(name +  "[" + to_string(light.renderQueue - 1) + "].linear", light.linear);
			setFloat(name +  "[" + to_string(light.renderQueue - 1) + "].quadratic", light.quadratic);
			setFloat(name +  "[" + to_string(light.renderQueue - 1) + "].constant", light.constant);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].specular", light.specular.x, light.specular.y, light.specular.z);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].ambient", light.ambient.x, light.ambient.y, light.ambient.z);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].position", light.position.x + parentObjPos.x, light.position.y + parentObjPos.y, light.position.z + parentObjPos.z);
			setVector(name + "[" + to_string(light.renderQueue - 1) + "].direction", light.direction.x, light.direction.y, light.direction.z);
			setFloat(name +  "[" + to_string(light.renderQueue - 1) + "].cutoff", cos(radians(light.cutoff)));
			setFloat(name +  "[" + to_string(light.renderQueue - 1) + "].outerCutoff", cos(radians(light.outerCutoff)));
			break;
		case 3:
			name = "aLight";
			setVector(name + ".colour", light.colour.x, light.colour.y, light.colour.z, light.colour.w);
			glClearColor(light.colour.x / 15 * light.colour.w, light.colour.y / 15 * light.colour.w, light.colour.z / 15 * light.colour.w, 1);
			break;
		}

	}
	catch (exception e) {}

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
	name_ = objectName;
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
#endif