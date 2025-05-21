#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 direction;

out VS_OUT {
    vec3 colour;
    vec3 dir;
} vs_out;


void main()
{
    vec4 pos = projection * view * model * vec4(aPos, 1.0f) ;
    gl_Position = pos;
    vs_out.dir = vec3(projection * view * model * direction);
}