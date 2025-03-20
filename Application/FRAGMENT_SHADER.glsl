#version 330 core

out vec4 FragColor;

in vec4 vertex_colour;
in vec4 vertex_position_frag;
in vec2 TexCoord;

uniform float time;
uniform sampler2D MinionTexture;
uniform sampler2D OtherTexture;
uniform float mixAmount;
uniform bool wireframe;
uniform bool error;

uniform vec3 wireframeCol = vec3(1, 1, 1);  //White by default

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
        //FragColor = texture(MinionTexture, TexCoord);
    
        if (error)
        {
            FragColor = mix(texture(MinionTexture, TexCoord), Checkerboard(), 0.5f); 
            //FragColor = Checkerboard();
        }
        else
        {
            FragColor = texture(MinionTexture, TexCoord);
        }
        
    }
     
     //FragColor += -vertex_colour;
     //FragColor = vec4(TexCoord, 0, 0);
     //FragColor = vertex_colour;
     //FragColor = vec4(abs(vertex_position_frag)) * vertex_colour;
};