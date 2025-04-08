#version 330 core

out vec4 FragColor;

in vec3 Position;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform float time;
uniform float mixAmount;
uniform bool wireframe;
uniform bool error;
uniform vec4 cameraPos;

uniform vec3 wireframeCol = vec3(1, 1, 1);  //White by default

struct Light
{
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
    vec4 colour;
};
uniform Light globalSceneLight;

struct Material
{
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
    vec4 colour;
    sampler2D texture;
    bool as_tex;
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
            FragColor = mix(texture(mat.texture, TexCoord), Checkerboard(), 0.5f); 
            //FragColor = Checkerboard();
        }
        else
        {
            //diffuse
            vec3 normal = normalize(Normal);
            vec3 globalLightDir = normalize(vec3(globalSceneLight.position.x, globalSceneLight.position.y, globalSceneLight.position.z) - FragPos);
            float diff = max(dot(normal, globalLightDir), 0.0f);
            vec4 diffuse = globalSceneLight.diffuse * (diff * mat.diffuse);

            //specular
            vec3 viewDir = normalize(vec3(cameraPos.x, cameraPos.y, cameraPos.z) - FragPos);
            vec3 reflectDir = reflect(-globalLightDir, normal);
            float f_specular = pow(max(dot(viewDir, reflectDir),0), mat.shininess * 128);
            vec4 specular = globalSceneLight.specular * (f_specular * mat.specular);

            //ambient
            vec4 ambient = globalSceneLight.ambient * mat.ambient;
            vec4 pixelColours;
            if (mat.as_tex)
            {
               pixelColours = (ambient + diffuse) * texture(mat.texture, TexCoord) + specular;
            }
            else
            {
               pixelColours = ambient + diffuse + specular; 
            }

            //FragColor = vec4(normal * 0.5 + 0.5, 1.0);w
            FragColor = pixelColours * mat.colour * globalSceneLight.colour * globalSceneLight.colour.w;
        }
        
    }
     
     //FragColor += -vertex_colour;
     //FragColor = vec4(TexCoord, 0, 0);
     //FragColor = vertex_colour;
     //FragColor = vec4(abs(vertex_position_frag)) * vertex_colour;
};