#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aCol;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 mat4_model;
uniform mat4 mat4_perspective;
uniform mat4 mat4_camera;

uniform vec2 renderRes;

out vec3 fragment_position;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_colour;
out vec2 v_texCoord;

void main() {
	v_position = aPos;
	v_normal = -mat3(transpose(inverse(mat4_model))) * aNormal;  
	v_colour = aCol;
	v_texCoord = aTexCoord;

	vec4 pos = mat4_perspective * mat4_camera * mat4_model * vec4(aPos, 1.0f);
	fragment_position =  (mat4_model * vec4(aPos, 1.0f)).xyz; 
	gl_Position = pos;
}