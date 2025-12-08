#version 430
out vec4 FragColor;

struct Material {
	sampler2D diffuse_tex;
};
uniform Material mat; 

float nearClip = 0.1;
float farClip  = 100;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; 
    return (2.0 * nearClip * farClip) / (farClip + nearClip - z * (farClip - nearClip));
}
void main() {
	float depth = LinearizeDepth(gl_FragCoord.z) / farClip;
	FragColor = vec4(vec3(depth), 1);
}