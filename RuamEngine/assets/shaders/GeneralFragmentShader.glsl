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

    //sampler2D tex = textures[frag_texID];
    //final_color = texture(tex, frag_uv);
    if (frag_texID == 0)
        final_color = vec4(1,0,0,1); // rojo
    else if (frag_texID == 1)
        final_color = vec4(0,1,0,1); // verde
    else if (frag_texID == 2)
        final_color = vec4(0,0,1,1); // azul
    else if (frag_texID == 3)
        final_color = vec4(1,1,1,1);
    else
        final_color = vec4(1,0,1,1); // magenta (índice inesperado)

    return;
    if (frag_texID < 0 || frag_texID >= int(textures.length())) 
    {
            final_color = vec4(1,0,1,1);
            return;
    }


}
