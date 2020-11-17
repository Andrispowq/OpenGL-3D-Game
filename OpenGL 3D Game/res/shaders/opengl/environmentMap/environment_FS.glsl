#version 430

layout (location = 0) out vec4 outColour;

layout (location = 0) in vec3 position_FS;

uniform samplerCube environmentMap;
uniform float gamma;

void main()
{
	vec3 envColour = textureLod(environmentMap, normalize(vec3(position_FS.x, position_FS.y, -position_FS.z)), 0.2).rgb;
	
	envColour = envColour / (envColour + vec3(1.0));
	envColour = pow(envColour, vec3(1.0 / gamma));
	
	outColour = vec4(envColour, 1);
}