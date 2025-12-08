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

in vec2 screenspace_coord;
uniform vec2 renderRes;

void main() {
	FragColor = (texture(mat.diffuse_tex, gl_FragCoord.xy / renderRes) * vec4(mat.diffuse.xyz, 1));
}