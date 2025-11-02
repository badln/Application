#ifndef KTY_SHADER
#define KTY_SHADER

#include "EngineMaths.h"
#include "Logger.h"
#include <assimp/types.h>
#include <glm/gtc/type_ptr.hpp>


#define STR(name) (#name)

class ShaderProgram {
public:
	DebugLogger ShaderLog;
	const char* ShaderFolderName = "Shaders\\";
	ShaderProgram CreateShader(const char* VERTEX_SHADER_PATH, const char* FRAGMENT_SHADER_PATH);
	GLuint shaderID;
	void checkCompileErrors(unsigned int shader, std::string type);
	void Use();
	template <typename T>
	void SetVar(T var, const char* varName) {}
	template <> void SetVar<int>(int var, const char* varName) {
		glUniform1i(glGetUniformLocation(shaderID, varName), var);
	}
	template <> inline void SetVar<float>(float var, const char* varName) {
		glUniform1f(glGetUniformLocation(shaderID, varName), var);
	}
	template <> inline void SetVar<bool>(bool var, const char* varName) {
		glUniform1i(glGetUniformLocation(shaderID, varName), var);
	}
	template <> inline void SetVar<GLuint>(GLuint var, const char* varName) {
		glUniform1ui(glGetUniformLocation(shaderID, varName), var);
	}
	template <> inline void SetVar<Vector2>(Vector2 var, const char* varName) {
		glUniform2f(glGetUniformLocation(shaderID, varName), var.x(), var.y());
	}
	template <> inline void SetVar<Vector3>(Vector3 var, const char* varName) {
		glUniform3f(glGetUniformLocation(shaderID, varName), var.x(), var.y(), var.z());
	}
	template <> inline void SetVar<Vector4>(Vector4 var, const char* varName) {
		glUniform4f(glGetUniformLocation(shaderID, varName), var.x(), var.y(), var.z(), var.w());
	}
	template <> inline void SetVar<glm::mat4>(glm::mat4 var, const char* varName) {
		glUniformMatrix4fv(glGetUniformLocation(shaderID, varName), 1, GL_FALSE, glm::value_ptr(var));
	}
	template <> inline void SetVar<glm::mat3>(glm::mat3 var, const char* varName) {
		glUniformMatrix3fv(glGetUniformLocation(shaderID, varName), 1, GL_FALSE, glm::value_ptr(var));
	}
	template <> inline void SetVar<glm::mat2>(glm::mat2 var, const char* varName) {
		glUniformMatrix2fv(glGetUniformLocation(shaderID, varName), 1, GL_FALSE, glm::value_ptr(var));
	}
private:
	void stripUnicode(std::string& str)
	{
		str.erase(remove_if(str.begin(), str.end(), [](char c) {return !(c >= 0 && c < 128); }), str.end());
	}
};

#endif