#version 450 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

const int SSBOType_textures = 3;

layout(binding = SSBOType_textures, std430) readonly buffer ssbo3
{
    sampler2D textures[];
};

smooth in vec2 frag_uv;
flat in int frag_instance;

out vec4 final_color;

flat in int frag_texID;

uniform vec4  u_albedoColor;
uniform float u_diffuse;
uniform float u_specular;

void main()
{
    sampler2D tex = textures[frag_texID];
    final_color = texture(tex, frag_uv);   
}
