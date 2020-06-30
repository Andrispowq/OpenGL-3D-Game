#version 450

layout (location = 0) in vec3 position_VS;
layout (location = 1) in vec2 texture_VS;
layout (location = 2) in vec3 normal_VS;

layout (location = 0) out vec2 texture_GS;
layout (location = 1) out vec3 normal_GS;

layout(set = 2, binding = 0) uniform Model
{
	layout(offset = 0) mat4 m_transform;
};

void main()
{
	gl_Position = m_transform * vec4(position_VS, 1.0);
	gl_Position.y = -gl_Position.y;
    //gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

	texture_GS = texture_VS;
	normal_GS = (m_transform * vec4(normal_VS, 0.0)).xyz;
}