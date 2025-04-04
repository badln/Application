#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 Position;

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
   FragPos = vec3(model * vec4(aPos, 1.0f));
   gl_Position = perspective * view * vec4(FragPos, 1.0f);
   if (truncVerts)
   {
       gl_Position = vec4(
			truncate(gl_Position.x, vertTruncAmount),
			truncate(gl_Position.y, vertTruncAmount),
			truncate(gl_Position.z, vertTruncAmount),
			truncate(gl_Position.w, vertTruncAmount)
			);
   }
   TexCoord = aTexCoord;
   Normal = mat3(transpose(inverse(model))) * aNormal;
   Position = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
};