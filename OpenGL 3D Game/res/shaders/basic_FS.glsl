#version 330

layout(location = 0) out vec4 out_Colour;

in vec3 colour_FS;

void main()
{
	out_Colour = vec4(colour_FS, 1);
}