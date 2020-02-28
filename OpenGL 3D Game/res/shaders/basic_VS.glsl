#version 330

layout(location = 0) in vec3 position_VS;
layout(location = 1) in vec2 texture_VS;
layout(location = 2) in vec3 normal_VS;

out vec2 texture_FS;

uniform mat4 m_transform;
uniform mat4 m_view;
uniform mat4 m_projection;

void main()
{
	gl_Position = /*m_projection * m_view **/ m_transform * vec4(position_VS, 1);

	texture_FS = texture_VS;
}