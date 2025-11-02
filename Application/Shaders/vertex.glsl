#version 430 core
layout(location = 0) in vec3 aPos;

uniform mat4 ModelTransform;
uniform mat4 ProjectionMatrix;

void main() {
	gl_Position = ProjectionMatrix * ModelTransform * vec4(aPos, 1.0f);
}