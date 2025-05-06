#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 colour;
} vs_out;


void main()
{
    vec4 pos = projection * view * model * vec4(aPos, 1.0f) ;
    gl_Position = pos;
}