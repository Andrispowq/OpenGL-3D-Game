#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 colour_Out;

layout(location = 0) in vec3 colour_FS;
layout(location = 1) in vec2 texture_FS;

layout(set = 0, binding = 1) uniform sampler2D texSampler;

void main() 
{
    colour_Out = texture(texSampler, texture_FS);
}