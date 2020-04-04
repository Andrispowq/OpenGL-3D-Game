#version 450
#extension GL_KHR_vulkan_glsl: enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position_VS;
layout(location = 1) in vec2 texture_VS;
layout(location = 2) in vec3 normal_VS;
layout(location = 3) in vec3 colour_VS;

layout(location = 0) out vec3 colour_FS;

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position_VS, 1.0);

    colour_FS = colour_VS;
}