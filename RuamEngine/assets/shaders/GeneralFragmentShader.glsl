#version 450 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

const int SSBOType_textures = 3;

layout(binding = SSBOType_textures, std430) readonly buffer ssbo3
{
    uvec2 textures[];
};

smooth in vec2 frag_uv;
flat in int frag_instance;

out vec4 final_color;

flat in int frag_texID;

uniform vec4  u_albedoColor;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ambientOcclusion;
uniform float u_emissiveStrength;
    
//uniform sampler2D u_albedoMap[32];
//uniform sampler2D u_normalMap;
//uniform sampler2D u_metallicMap;
//uniform sampler2D u_roughnessMap;
//uniform sampler2D u_AOMap;
//uniform sampler2D u_emissiveMap;

void main()
{
//    uvec2 handle = textures[frag_texID];
//    sampler2D tex = sampler2D(handle);
//    final_color = vec4(texture(tex, frag_uv).rgb, 1.0);

//    uvec2 h0 = textures[0];
//    sampler2D s0 = sampler2D(h0);
//    vec4 c0 = texture(s0, frag_uv);
//
//    uvec2 h1 = textures[1];
//    sampler2D s1 = sampler2D(h1);
//    vec4 c1 = texture(s1, frag_uv);
//
//    // Para debug: si frag_texID==0 devuelve c0, si ==1 devuelve c1
//    if (frag_texID == 0) final_color = c0;
//    else if (frag_texID == 1) final_color = c1;
//    else final_color = vec4(1,0,1,1); // magenta para ver indexes inválidos


// ---------
//    if (frag_texID < 0 || frag_texID >= textures.length()) 
//    {
//        final_color = vec4(1,0,1,1); // magenta por debug
//        return;
//    }
//    else
//    {
//        final_color = vec4(1,1,0,1);
//    }

// --------

    if (frag_texID < 0 || frag_texID >= int(textures.length())) {
            final_color = vec4(1,0,1,1);
            return;
        }

        uvec2 h = textures[frag_texID];
        sampler2D tex = sampler2D(h); // constructor acepta uvec2
        final_color = texture(tex, frag_uv);
}
