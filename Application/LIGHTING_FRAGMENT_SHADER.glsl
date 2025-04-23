#version 330 core

out vec4 FragColor;

in vec3 Position;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform float time;
uniform float mixAmount;
uniform bool wireframe;
uniform vec4 cameraPos;

uniform vec3 wireframeCol = vec3(1, 1, 1);  //White by default

struct Light
{
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
    vec4 colour;

    vec4 direction;
    int type;

    float linear;
    float quadratic;
    float constant;

    float cutoff;
    float outerCutoff;
};
uniform Light light;

struct Material
{
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
    float emmissive;
    vec4 colour;

    sampler2D texture;
    bool as_tex;
    sampler2D diffuseTex;
    bool dif_tex;
    sampler2D specularTex;
    bool spec_tex;
    sampler2D emmissiveTex;
    bool em_tex;
    bool texError;
};
uniform Material mat;

float timeMultiplier = 2.5f;
float flip(float num)
{
    return abs(num - 1);
}

vec4 Checkerboard()
{
    vec2 uv = TexCoord;
    
    vec4 col1 = vec4(round(uv.x), round(uv.x), round(uv.x), 1); 
    vec4 col2 = vec4(flip(round(uv.y)), flip(round(uv.y)), flip(round(uv.y)), 1); 
    vec4 col3 = vec4(round(uv.y), round(uv.y), round(uv.y), 1); 
    vec4 col4 = vec4(flip(round(uv.x)), flip(round(uv.x)), flip(round(uv.x)), 1); 
    
    vec4 frame1 = ((col3 * col4) + (col2 * col1)) * vec4(1, 0, 1, 1);
    vec4 frame2 = ((col1 * col3) + (col4 * col2)) * vec4(1, 0, 1, 1);
    if (sin(time * timeMultiplier) >= 0)
    {
        return frame1;
    }
    else
    {
        return frame2;
    } 
}

void main()
{
    if (wireframe)
    {
        FragColor = vec4(wireframeCol, 1);
    }
    else
    {
        if (mat.texError)
        {
            FragColor = mix(texture(mat.texture, TexCoord), Checkerboard(), 0.5f); 
            //FragColor = Checkerboard();
        }
        else
        {
            vec4 diffuseTexture;
            vec4 specularTexture;
            vec4 emmissiveTexture;
            if (mat.dif_tex)
            {
                diffuseTexture = vec4(texture(mat.diffuseTex, TexCoord));
            }
            else
            {
                diffuseTexture = vec4(1.0f);
            }
            if (mat.spec_tex)
            {
                specularTexture = vec4(texture(mat.specularTex, TexCoord));
            }
            else
            {
                specularTexture = vec4(1.0f);
            }
            if (mat.em_tex)
            {
                emmissiveTexture = vec4(texture(mat.emmissiveTex, TexCoord));
            }
            else
            {
                emmissiveTexture = vec4(0.0f);
            }

            //diffuse
            vec3 normal = normalize(Normal);
            vec3 lightDir;
            vec4 pixelColours;
            lightDir = normalize(vec3(light.position) - FragPos);
            if (light.type == 1){

                lightDir = normalize(vec3(-light.direction));
            }
           
            float diff = max(dot(normal, lightDir), 0.0f);
            vec4 diffuse = light.diffuse * (diff * mat.diffuse * diffuseTexture);

            //specular
            vec3 viewDir = normalize(vec3(cameraPos.x, cameraPos.y, cameraPos.z) - FragPos);
            vec3 reflectDir = reflect(-lightDir, normal);
            float f_specular = pow(max(dot(viewDir, reflectDir),0), mat.shininess * 128);
            vec4 specular = light.specular * (f_specular * mat.specular * specularTexture);

            //ambient
            vec4 ambient = light.ambient * mat.ambient * diffuseTexture;

            if (light.type == 2){
                

                float theta = dot(lightDir, normalize(vec3(-light.direction)));
                float epsilon = light.cutoff - light.outerCutoff;
                float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);    

                diffuse *= intensity;
                specular *= intensity;

            }

            float distance = length(vec3(light.position) - FragPos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;

            if (mat.as_tex)
            {
               pixelColours = (ambient + diffuse) * texture(mat.texture, TexCoord) + specular + emmissiveTexture;
            }
            else
            {
               pixelColours = ambient + diffuse + specular + emmissiveTexture; 
            }
            //FragColor = emmissiveTexture;
            FragColor = pixelColours * mat.colour * light.colour * (light.colour.w + mat.emmissive);
        }
        
    }
     
     //FragColor += -vertex_colour;
     //FragColor = vec4(TexCoord, 0, 0);
     //FragColor = vertex_colour;
     //FragColor = vec4(abs(vertex_position_frag)) * vertex_colour;
};