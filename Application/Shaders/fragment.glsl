#version 430 core
out vec4 FragColor;

uniform vec3 newCol = vec3(1); 

void main() {
	FragColor = vec4(newCol, 1);
}