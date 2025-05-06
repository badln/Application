#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 64) out;
out vec3 fColour;

uniform float time;
uniform float windowSizeX = 1;
uniform float windowSizeY = 1;

float radian = 6.28319f;

int circleSteps = 16;
int sunBeamSteps = 7;
int sunCircleSteps = 10;

uniform vec4 colourMultiply = vec4(1);

in VS_OUT {
    vec3 colour;
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

void GeneratePointLight()
{
    fColour = vec3(0.8f) * colourMultiply.xyz;
    
    vec2 ratio = vec2(windowSizeX / windowSizeY, 1);
    vec2 rotationVector = vec2(0.1f, 0.0f);
    
    float stepAngleRad = radian / (circleSteps - 1);
    for (int i = 0; i < circleSteps; i++)
    {
        rotationVector = rotateAround(rotationVector, vec2(0), stepAngleRad);
        gl_Position = gl_in[0].gl_Position + vec4(rotationVector / ratio, 0.0f, 0.0f) * 2.5;
        EmitVertex();
    }
    EndPrimitive();
    rotationVector = rotateAround(vec2(0.1f, 0.0f), vec2(0), radian / 4);
    fColour = vec3(1, 1, 0.5) * colourMultiply.xyz;
    stepAngleRad = radian / (sunBeamSteps);
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

void main() {   

    GeneratePointLight();
}  