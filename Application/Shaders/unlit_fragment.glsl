#version 430 core
out vec4 FragColor;

struct Material {
	
	sampler2D diffuse_tex;
    sampler2D specular_tex;
	sampler2D emissive_tex;
	sampler2D normal_tex;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;

};

uniform Material mat;

in vec3 v_position;
in vec3 v_normal; 
in vec3 v_colour;
in vec2 v_texCoord;

void main() {
	FragColor = (texture(mat.diffuse_tex, v_texCoord) * vec4(mat.diffuse.xyz, 1));
}