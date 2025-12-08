#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depth;

float nearClip = 0.1;
float farClip  = 1000;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; 
    return (2.0 * nearClip * farClip) / (farClip + nearClip - z * (farClip - nearClip));
}
void main() {
	vec4 depthTex = texture(depth, TexCoords);
	float depth = LinearizeDepth(depthTex.r) / farClip;
	if (depth >= 1)
		depth = 0;
	FragColor = vec4(vec3(depth), 1);
}