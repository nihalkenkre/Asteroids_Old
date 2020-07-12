#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (set = 1, binding = 0) uniform sampler2D base_color[16];

layout (push_constant) uniform image_index
{
    int index;
} constants;


layout (location = 0) in vec2 in_uv;

layout (location = 0) out vec4 out_color;

void main ()
{
    out_color = vec4 (1,0,1,1);
}    
