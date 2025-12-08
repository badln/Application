#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
	sampler2D diffuse_tex;
};
uniform Material mat; 

void main() {
	FragColor = texture(mat.diffuse_tex, TexCoords);
}