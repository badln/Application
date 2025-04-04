#version 330 core

out vec4 FragColor;

in vec3 Position;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform float time;
uniform sampler2D Texture;
uniform sampler2D OtherTexture;
uniform float mixAmount;
uniform bool wireframe;
uniform bool error;
uniform vec3 cameraPos;

uniform vec4 globalSceneLight;
uniform vec4 globalSceneLightPos;

uniform vec3 wireframeCol = vec3(1, 1, 1);  //White by default

struct Material
{
    float ambient;
	float diffuse;
	float specular;
	float shininess;
    vec4 colour;
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
        if (error)
        {
            FragColor = mix(texture(Texture, TexCoord), Checkerboard(), 0.5f); 
            //FragColor = Checkerboard();
        }
        else
        {
            //diffuse
            float diffuseAmount = mat.diffuse * globalSceneLight.w;
            vec3 normal = normalize(Normal);
            vec3 globalLightDir = normalize(vec3(globalSceneLightPos.x, globalSceneLightPos.y, globalSceneLightPos.z) - FragPos);
            float diff = max(dot(normal, globalLightDir), 0.0f);
            vec4 diffuse = diff * globalSceneLight * diffuseAmount * globalSceneLight.w;

            //specular
            float specularStrength = mat.specular * globalSceneLight.w;
            vec3 viewDir = normalize(cameraPos - FragPos);
            vec3 reflectDir = reflect(-globalLightDir, normal);
            float f_specular = pow(max(dot(viewDir, reflectDir),0),32);
            vec4 specular = f_specular * globalSceneLight * specularStrength;

            //ambient
            float ambientStrength = mat.ambient * globalSceneLight.w;
            vec4 ambient = ambientStrength * globalSceneLight;
            vec4 pixelColours = texture(Texture, TexCoord);
            
            pixelColours = (ambient + diffuse + specular) * pixelColours;
            pixelColours *= mat.colour;

            //FragColor = vec4(normal * 0.5 + 0.5, 1.0);w
            FragColor = vec4(pixelColours.x, pixelColours.y, pixelColours.z, 1.0f);
        }
        
    }
     
     //FragColor += -vertex_colour;
     //FragColor = vec4(TexCoord, 0, 0);
     //FragColor = vertex_colour;
     //FragColor = vec4(abs(vertex_position_frag)) * vertex_colour;
};