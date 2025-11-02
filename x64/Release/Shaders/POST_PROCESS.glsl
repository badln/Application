#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform vec4 colour = vec4(1);
uniform sampler2D tex;

uniform float time;

float quantizeLevel = 24;

void main()
{
	float levels = quantizeLevel;// + abs((sin(time) + 1) / 5) + abs((sin(time * 30)) / 30);
	FragColor = texture(tex, TexCoords);
	float greyscale = max(FragColor.r, max(FragColor.g, FragColor.b));

	float lower     = floor(greyscale * levels) / levels;
    float dif_lower = abs(greyscale - lower);

	float higher    = ceil(greyscale * levels) / levels;
	float dif_higher= abs(higher - greyscale);
	 
	float level      = dif_lower <= dif_higher ? lower : higher;
    float adjustment = level / greyscale;	

	FragColor.xyz *= adjustment;
	//FragColor = texture(tex, TexCoords);
}