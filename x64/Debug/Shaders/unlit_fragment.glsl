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

in vs_out {
	vec3 v_position;
	vec3 v_normal; 
	vec3 v_colour;
	vec2 v_texCoord;
} vs_in;

void main() {
	FragColor = (texture(mat.diffuse_tex, vs_in.v_texCoord) * vec4(mat.diffuse.xyz, 1));
}