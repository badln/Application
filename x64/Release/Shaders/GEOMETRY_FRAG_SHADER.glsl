#version 330 core

in vec3 fColour;
out vec4 FragColor;

void main()
{
	FragColor = vec4(fColour, 1);
}
