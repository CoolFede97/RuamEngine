#version 330 core

out vec4 final_color;

in vec2 frag_texCoords;
in float frag_texId;

uniform vec4  u_albedoColor;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ambientOcclusion;
uniform float u_emissiveStrength;

uniform sampler2D u_albedoMap;
//uniform sampler2D u_normalMap;
//uniform sampler2D u_metallicMap;
//uniform sampler2D u_roughnessMap;
//uniform sampler2D u_AOMap;
//uniform sampler2D u_emissiveMap;

void main()
{
    final_color = texture(u_albedoMap, frag_texCoords);
}
