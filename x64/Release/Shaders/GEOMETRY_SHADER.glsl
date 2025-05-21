#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 64) out;
out vec3 fColour;

uniform float time;
uniform float windowSizeX;
uniform float windowSizeY;
uniform int type;

float radian = 6.28319f;

int circleSteps = 16;
int sunBeamSteps = 7;
int sunCircleSteps = 10;

uniform float cutoff;
uniform float outerCutoff;
uniform vec4 colourMultiply = vec4(1);

vec2 ratio;

in VS_OUT {
    vec3 colour;
    vec3 dir;
} gs_in[]; 

vec2 rotateAround(vec2 point, vec2 origin, float theta){
    float s = sin(theta);
    float c = cos(theta);

    point -= origin;

    vec2 r;

    r.x = point.x * c - point.y * s;
    r.y = point.x * s + point.y * c;

    r += origin;

    return r;
}
void GenerateCircle(vec2 rotationVector, vec2 ratio)
{
    float stepAngleRad = radian / (circleSteps - 1);
    for (int i = 0; i < circleSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) * 2.5;
        EmitVertex();
    }
    EndPrimitive();
}
void GeneratePointLight()
{
    fColour = vec3(0.8f) * colourMultiply.xyz;
    
    vec2 rotationVector = vec2(0.1f, 0.0f);
    
    GenerateCircle(rotationVector, ratio);
    rotationVector = rotateAround(vec2(0.1f, 0.0f), vec2(0), radian / 4);
    fColour = vec3(1, 1, 0.5) * colourMultiply.xyz;
    float stepAngleRad = radian / (sunBeamSteps);
    for (int i = 0; i < sunBeamSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) * 1.5f;
        EmitVertex();
        EndPrimitive();
    }
    rotationVector = vec2(0.1f, 0.0f);
    stepAngleRad = radian / (sunCircleSteps - 1);
    for (int i = 0; i < sunCircleSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) * 0.6;
        EmitVertex();
    }
    EndPrimitive();
}

void GenerateDirectionalLight()
{
    float scale = 1.5f;

    vec2 rotationVector = vec2(0.1f, 0.0f);

    fColour = vec3(1, 1, 0.5) * colourMultiply.xyz;
    float stepAngleRad = radian / (sunCircleSteps - 1);
    for (int i = 0; i < sunCircleSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) + vec4(0.0f, 0.075f, 0, 0) * 1.2 * scale;
        EmitVertex();
    }
    EndPrimitive();
    
    fColour = vec3(0.8) * colourMultiply.xyz;
    float theta = 3 * (3.14159 / 180);
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(gs_in[0].dir.xy, 0, gs_in[0].dir.z) * 1.3;
    EmitVertex();
    EndPrimitive();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), theta), 0, gs_in[0].dir.z);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale), 0, 0) + vec4(gs_in[0].dir.xy, 0, gs_in[0].dir.z) * 1.3;
    EmitVertex();
     gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), -theta), 0, gs_in[0].dir.z);
    EmitVertex();
    EndPrimitive();
}
float radianBetween(vec2 pos1, vec2 pos2)
{
    float theta = acos((dot(pos1, pos2) / dot(normalize(pos1), normalize(pos2))));
    return theta;
}
void GenerateLine(float spotCutoff, float scale)
{
    for (int i = 0; i < 2; i++)
    {
       
        if (i == 1)
            spotCutoff = -spotCutoff;

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), spotCutoff), 0, gs_in[0].dir.z) * 100;
        EmitVertex();
        EndPrimitive();
    }
}
void GenerateSpotLight()
{
    float scale = 1.5f;

    vec2 rotationVector = vec2(0.1f, 0.0f);

    fColour = vec3(1, 1, 0.5) * colourMultiply.xyz;
    float stepAngleRad = radian / (sunCircleSteps - 1);
    for (int i = 0; i < sunCircleSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) + vec4(0.0f, 0.075f, 0, 0) * 1.2 * scale;
        EmitVertex();
    }
    EndPrimitive();

    fColour = vec3(0.8) * colourMultiply.xyz;

    float spotCutoff = clamp((outerCutoff + cutoff) / 4, 0, 90) * (3.14159 / 180);

    GenerateLine(spotCutoff, scale);

    fColour = vec3(0.8, 0, 0) * colourMultiply.xyz;
    spotCutoff = clamp((cutoff) / 2, 0, 90) * (3.14159 / 180);

    for (int i = 0; i < 2; i++)
    {
       
        if (i == 1)
        spotCutoff = -spotCutoff;

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), spotCutoff), 0, gs_in[0].dir.z) * 10;
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), spotCutoff), 0, gs_in[0].dir.z) * 20;
        EmitVertex();
        EndPrimitive();
    }
    fColour = vec3(0, 0.4, 0.8) * colourMultiply.xyz;
    spotCutoff = clamp((outerCutoff) / 2, 0, 90) * (3.14159 / 180);

    for (int i = 0; i < 2; i++)
    {
       
        if (i == 1)
            spotCutoff = -spotCutoff;

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), spotCutoff), 0, gs_in[0].dir.z) * 10;
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), spotCutoff), 0, gs_in[0].dir.z) * 20;
        EmitVertex();
        EndPrimitive();
    }

    fColour = vec3(0.8) * colourMultiply.xyz;
    float theta = 3 * (3.14159 / 180);
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(gs_in[0].dir.xy, 0, gs_in[0].dir.z) * 1.3;
    EmitVertex();
    EndPrimitive();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), theta), 0, gs_in[0].dir.z);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale), 0, 0) + vec4(gs_in[0].dir.xy, 0, gs_in[0].dir.z) * 1.3;
    EmitVertex();
     gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0f, 0.075f * 1.2 * scale) / ratio, 0, 0) + vec4(rotateAround(gs_in[0].dir.xy, vec2(gl_in[0].gl_Position.xy + vec2(0.0f, 0.075f * 1.2 * scale)), -theta), 0, gs_in[0].dir.z);
    EmitVertex();
    EndPrimitive();
}

void main() {   

    ratio = vec2(windowSizeX / windowSizeY, 1);
    switch (type)
    {
    case 0:
        GeneratePointLight();
        break;
    case 1:
        GenerateDirectionalLight();
        break;
    case 2:
        GenerateSpotLight();
        break;
    case 3:
        GeneratePointLight();
        break;
    }

}  