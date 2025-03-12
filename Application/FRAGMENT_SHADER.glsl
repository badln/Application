#version 330 core

out vec4 FragColor;

in vec4 vertex_colour;
in vec4 vertex_position_frag;
in vec2 TexCoord;

uniform sampler2D MinionTexture;
uniform sampler2D OtherTexture;
uniform float mixAmount;
uniform bool wireframe;

uniform vec3 wireframeCol = vec3(1, 1, 1);  //White by default

void main()
{
    if (wireframe)
    {
        FragColor = vec4(wireframeCol, 1);
    }
    else
    {
        FragColor = mix(texture(MinionTexture, TexCoord), texture(OtherTexture, TexCoord), mixAmount);
    }
     
     //FragColor += -vertex_colour;
     //FragColor = vec4(TexCoord, 0, 0);
     //FragColor = vertex_colour;
     //FragColor = vec4(abs(vertex_position_frag)) * vertex_colour;
};