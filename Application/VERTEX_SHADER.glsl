#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertex_colour;
out vec2 TexCoord;

uniform vec4 offset;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 perspective = mat4(1.0f);

uniform float vertTruncAmount = 10;
uniform bool truncVerts;

float truncate(float num, float place)
{
	return int(num * place) / place;
}

void main()
{
   gl_Position = mat4(2) * vec4(aPos.x, aPos.y, aPos.z, 1.0f); // + offset;
   gl_Position = perspective * view * model * gl_Position;
   if (truncVerts)
   {
       gl_Position = vec4(
			truncate(gl_Position.x, vertTruncAmount),
			truncate(gl_Position.y, vertTruncAmount),
			truncate(gl_Position.z, vertTruncAmount),
			truncate(gl_Position.w, vertTruncAmount)
			);
   }
   vertex_colour = vec4(aColour, 1.0f);
   TexCoord = aTexCoord;
};