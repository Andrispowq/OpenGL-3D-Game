#version 330

layout(location = 0) out vec4 out_Colour;

in vec2 texture_FS;

uniform sampler2D  diffuse;

void main()
{
	out_Colour = 1 - texture(diffuse, texture_FS);
}