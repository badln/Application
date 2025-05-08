#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform vec4 colour = vec4(1);
uniform sampler2D tex;

float r = 10;

void main()
{
	FragColor = round(texture(tex, TexCoords) * r) / r;
	//FragColor = texture(tex, TexCoords);
}