#version 330 

layout (location = 0) out vec4 outColour;

in vec2 texture_FS;
in float distanceFromLeft;

uniform samplerCube image;

uniform float progress;

void main()
{
	outColour = texture(image, vec3(texture_FS.x, texture_FS.y, 1)) * progress;
}