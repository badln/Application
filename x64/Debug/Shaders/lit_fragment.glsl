#version 430 core

out vec4 FragColor;

vec3 camera_position;

vec3 v_normal;
vec3 lightDir;

vec3 view_direction;
vec3 reflect_direction;
in vec3 fragment_position;

struct Material {
	
	sampler2D diffuse_tex;
    sampler2D specular_tex;
	sampler2D emissive_tex;
	sampler2D normal_tex;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;
	float alpha;

};

uniform Material mat;

struct Light {
    
	vec3 direction;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform int l_assignedPointLights;
uniform int l_assignedSpotLights;
uniform int l_assignedDirectionalLights;

const int MAX_LIGHTS = 32;

uniform Light pointLights[MAX_LIGHTS];

in vs_out {
	vec3 v_position;
	vec3 v_normal; 
	vec3 v_colour;
	vec2 v_texCoord;
} vs_in;


vec3 ambient(Light light) {
	return vec3(mat.ambient * light.ambient); 
}
vec3 emissive() {
	return texture(mat.emissive_tex, vs_in.v_texCoord).xyz * mat.emissive.xyz;
}
vec3 diffuse(Light light) {
	lightDir = normalize(light.position - fragment_position);
	float d = max(dot(v_normal, lightDir), 0.0);
	return d * (mat.diffuse.xyz * light.diffuse.xyz) * texture(mat.diffuse_tex, vs_in.v_texCoord).xyz + ambient(light);
}
vec3 specular(Light light) {
	view_direction = normalize(camera_position - fragment_position);
	reflect_direction = reflect(-lightDir, v_normal);	
	float spec = pow(max(dot(view_direction, reflect_direction), 0), mat.shininess);
	return spec * (mat.specular.xyz * (light.specular.xyz * light.diffuse.xyz)) * texture(mat.specular_tex, vs_in.v_texCoord).xyz;
}

void main() {

	v_normal = normalize(vs_in.v_normal * texture(mat.normal_tex, vs_in.v_texCoord).xyz);
	
	vec4 pLights = vec4(0,0,0,1);
	for (int i = 0; i < l_assignedPointLights; i++) {
		pLights += vec4(diffuse(pointLights[i]) + specular(pointLights[i]), 1);
	}
	FragColor = vec4(pLights.xyz * vs_in.v_colour.xyz + emissive().xyz, mat.alpha) * mat.alpha;
}