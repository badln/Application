#version 330 core

out vec4 FragColor;
#define MAX_TEXTURES 4
in vec3 Position;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform float time;
uniform float mixAmount;
uniform bool wireframe;
uniform vec4 cameraPos;
uniform vec3 wireframeCol = vec3(1);
uniform sampler2D test;

float near = 0.1f;
float far = 100.0f;

struct AmbientLight {
    
    vec4 colour;
};

struct DirLight {

    vec4 colour;
    vec4 direction;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec4 colour;

    vec4 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {

    vec4 colour;
    vec4 position;
    vec4 direction;
    float cutoff;
    float outerCutoff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;       
};
uniform DirLight dirLight;
uniform PointLight pLight[4];
uniform SpotLight sLight[4];
uniform AmbientLight aLight;

uniform sampler2D diffuseTextures[MAX_TEXTURES];
uniform sampler2D specularTextures[MAX_TEXTURES];
uniform sampler2D emissiveTextures[MAX_TEXTURES];

uniform bool diffuseTexturesAs[MAX_TEXTURES];
uniform bool specularTexturesAs[MAX_TEXTURES];
uniform bool emissiveTexturesAs[MAX_TEXTURES];

struct Material
{
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
    float emissive;
    vec4 colour;

    sampler2D texture;
    bool as_tex;
    sampler2D diffuseTex;
    bool dif_tex;
    sampler2D specularTex;
    bool spec_tex;
    sampler2D emissiveTex;
    bool em_tex;
    bool texError;
};
uniform Material mat;
float timeMultiplier = 2.5f;
float flip(float num)
{
    return abs(num - 1.0f);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 difTex, vec4 specTex)
{
    vec3 lightDir = normalize(-vec3(light.direction));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 ambient = vec3(light.ambient) * vec3(difTex);
    vec3 diffuse = vec3(light.diffuse) * diff * vec3(difTex);   
    vec3 specular = vec3(light.specular) * spec * vec3(specTex);
    return clamp((ambient + diffuse + specular), 0, 1) * vec3(light.colour) * max(dot(normal, lightDir), 0);
}  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 difTex, vec4 specTex)
{
    vec3 lightDir = normalize(vec3(light.position) - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    float distance = length(vec3(light.position) - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient = vec3(light.ambient) * vec3(difTex);
    vec3 diffuse = vec3(light.ambient) * diff * vec3(difTex);
    vec3 specular = vec3(light.specular) * spec * vec3(specTex);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return clamp((ambient + diffuse + specular), 0, 1) * vec3(light.colour) * max(dot(normal, lightDir), 0);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 difTex, vec4 specTex)
{
    vec3 lightDir = normalize(vec3(light.position) - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    float distance = length(vec3(light.position) - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(vec3(-light.direction))); 
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = vec3(light.ambient) * vec3(difTex);
    vec3 diffuse = vec3(light.ambient) * diff * vec3(difTex);
    vec3 specular = vec3(light.specular) * spec * vec3(specTex);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return clamp((ambient + diffuse + specular), 0, 1) * vec3(light.colour) * max(dot(normal, lightDir), 0);
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

float LinearizeDepth(float depth)
{
    float z = depth * 2 - 1;
    return (2 * near * far) / (far + near - z * (far - near));
}

void main()
{

    {
        if (mat.texError)
        {
            FragColor = mix(texture(mat.texture, TexCoord), Checkerboard(), 0.5f); 
            //FragColor = Checkerboard();
        }
        else
        {
            vec3 pixelColours;
            vec4 emTex;
            vec4 specTex;
            vec4 difTex;
            vec4 tex;

            if (mat.em_tex)
            {   emTex = texture(mat.emissiveTex, TexCoord);
                if (emTex.a < 0.1f)
                    discard;}
            else { emTex = vec4(0.0f); }

            if (mat.as_tex)
            {   tex = texture(mat.texture, TexCoord);
                if (tex.a < 0.1f)
                    discard;}
            else { tex = vec4(1.0f); }

            if (mat.dif_tex)
            {   difTex = texture(mat.diffuseTex, TexCoord); 
                if (difTex.a < 0.1f)
                    discard;
                    }
            else { difTex = vec4(1.0f);}

            if (mat.spec_tex)
            {   specTex = texture(mat.specularTex, TexCoord);
                if (specTex.a < 0.1f)
                    discard;}
            else { specTex = vec4(1.0f); } 

            vec3 norm = normalize(Normal);
            vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
           
           
            for (int i = 0; i < MAX_TEXTURES; i++)
            {
                vec4 thisDifTex  = texture(diffuseTextures[i],  TexCoord);
                vec4 thisSpecTex = texture(specularTextures[i], TexCoord);
                vec4 thisEmTex   = texture(emissiveTextures[i], TexCoord);

                if (diffuseTexturesAs[i]){
                    //if (thisDifTex.a < 0.1f)
                        //discard;
                    difTex *= thisDifTex;}

                if (specularTexturesAs[i]){;
                    if (thisSpecTex.a < 0.1f)
                        discard;
                    specTex = thisSpecTex;}

                if (emissiveTexturesAs[i]){
                    if (thisEmTex.a < 0.1f)
                        discard;
                    emTex *= thisEmTex;}
            }
            pixelColours = CalcDirLight(dirLight, norm, viewDir, difTex, specTex);
            for (int i = 0; i < 4; i++)
            {
                pixelColours += CalcPointLight(pLight[i], norm, FragPos, viewDir, difTex, specTex);
            }
            for (int i = 0; i < 4; i++)
            {
                pixelColours += CalcSpotLight(sLight[i], norm, FragPos, viewDir, difTex, specTex);
            }
            FragColor = vec4(pixelColours, 1);// vec4(pixelColours, 1.0f) * ((mat.colour + emTex + aLight.colour / 10) );
        }
        
    }
     
     //FragColor = vec4(TexCoord, 0, 0);                                              //self explanitory
     //FragColor = vec4(FragPos, 1);                         

     //float depth = LinearizeDepth(gl_FragCoord.z) / far;                            //depth test
     //FragColor = vec4(vec3(depth), 1);

     //FragColor = vec4(normalize(Normal) / 2, 1);                                                     //Test normals
     //FragColor = vec4(dot(normalize(Normal), normalize(vec3(sLight[0].position) - FragPos)));  
};