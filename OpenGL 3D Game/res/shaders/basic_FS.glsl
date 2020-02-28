#version 330

layout(location = 0) out vec4 out_Colour;

in vec2 texture_FS;

void main()
{
	out_Colour = vec4(gl_FragCoord.x, 1 - gl_FragCoord.y, 0, 1);
}