#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position_VS;
layout(location = 1) in vec2 texture_VS;
layout(location = 2) in vec3 normal_VS;
layout(location = 3) in vec3 colour_VS;

layout(location = 0) out vec3 colour_FS;

layout(set = 0, binding = 0) uniform UBO1 { mat4 model; } ubo1;
layout(set = 0, binding = 1) uniform UBO2 { mat4 view; } ubo2;
layout(set = 0, binding = 2) uniform UBO3 { mat4 proj; } ubo3;

/*layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;*/

void main() 
{
    gl_Position = ubo3.proj * /*ubo2.view * ubo1.model **/ vec4(position_VS, 1.0);

    colour_FS = colour_VS;
}