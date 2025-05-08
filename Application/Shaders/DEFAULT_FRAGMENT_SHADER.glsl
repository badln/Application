#version 430 core
out vec4 FragColor;

uniform vec4 colour = vec4(1);

void main()
{
	FragColor = colour;
}