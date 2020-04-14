#version 330 

layout (location = 0) out vec4 outColour;

in vec2 texture_FS;

uniform sampler2D image;

void main()
{
	outColour = texture(image, texture_FS);
}