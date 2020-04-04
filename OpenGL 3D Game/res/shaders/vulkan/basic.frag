#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 colour_Out;

layout(location = 0) in vec3 colour_FS;


void main() 
{
    colour_Out = vec4(colour_FS, 1.0);
}