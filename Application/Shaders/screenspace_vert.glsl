#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 mat4_model;
out vec2 TexCoords;

void main()
{
    gl_Position = mat4_model * vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  