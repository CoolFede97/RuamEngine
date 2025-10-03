#version 450 core

#extension GL_ARB_bindless_texture : require

const int SSBOType_textures = 3;

layout(binding = SSBOType_textures, std430) readonly buffer ssbo3
{
    sampler2D textures[];
};

smooth in vec2 frag_uv;
flat in int frag_instance;

out vec4 final_color;

in vec2 frag_texCoords;
flat in int frag_texID;

uniform vec4  u_albedoColor;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ambientOcclusion;
uniform float u_emissiveStrength;
    
uniform sampler2D u_albedoMap[32];
//uniform sampler2D u_normalMap;
//uniform sampler2D u_metallicMap;
//uniform sampler2D u_roughnessMap;
//uniform sampler2D u_AOMap;
//uniform sampler2D u_emissiveMap;

void main()
{
    sampler2D tex = textures[frag_texID];
    final_color = vec4(texture(tex, frag_uv).rgb, 1.0);
}
