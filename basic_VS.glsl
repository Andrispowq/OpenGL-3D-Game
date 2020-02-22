#version 330

layout(location = 0) in vec3 position_VS;
layout(location = 1) in vec2 texture_VS;
layout(location = 2) in vec3 normal_VS;

out vec3 colour_FS;

uniform float scale;

void main()
{
	gl_Position = vec4(scale == 0 ? position_VS : position_VS * scale, 1);

	colour_FS = position_VS;
}