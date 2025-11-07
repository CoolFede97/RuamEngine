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

smooth in vec3 frag_normal;

//uniform vec4  u_albedoColor;
uniform float u_diffuse;
uniform float u_specular;
uniform float u_reflection;

void main()
{
    // sample the textures to get colors
    vec4 diffuse = texture(textures[int(u_diffuse)], frag_uv);
    vec4 specular = texture(textures[int(u_specular)], frag_uv);
    vec4 reflection = texture(textures[int(u_reflection)], frag_uv);

    final_color = (diffuse * 0.8 + specular * 0.2)*reflection;  
}
