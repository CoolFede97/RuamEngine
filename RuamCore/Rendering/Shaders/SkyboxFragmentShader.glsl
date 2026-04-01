#version 450 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

const int SSBOType_cubemaps = 4;

layout(binding = SSBOType_cubemaps, std430) readonly buffer ssbo3
{
    samplerCube textures[];
};

out vec4 frag_color;

in vec3 frag_texCoords;

uniform float u_cubemap;

void main()
{
    frag_color = texture(textures[uint(u_cubemap)], frag_texCoords);
}
