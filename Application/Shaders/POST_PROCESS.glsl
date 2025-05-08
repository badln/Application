#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec4 colour = vec4(1);
uniform sampler2D tex;

void main()
{
	FragColor = round(texture(tex, TexCoords) * 10) * 0.1;
}