#version 450 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

const int SSBOType_textures = 3;

layout(binding = SSBOType_textures, std430) readonly buffer ssbo3
{
    sampler2D textures[];
};

smooth in vec4 frag_pos;
smooth in vec2 frag_uv;
flat in int frag_instance;


out vec4 final_color;

smooth in vec3 frag_normal;

uniform vec4  u_baseColor;
uniform float u_diffuse;
uniform float u_specular;
uniform float u_reflection;
uniform float u_shininess;

uniform vec4 u_globalLightColor;
uniform vec3 u_globalLightPos;
uniform float u_lightOffset;

void main()
{
    // sample the textures to get colors
    vec4 diffuse = texture(textures[int(u_diffuse)], frag_uv);
    vec4 specular = texture(textures[int(u_specular)], frag_uv);
    vec4 reflection = texture(textures[int(u_reflection)], frag_uv);

    vec4 lightColor = vec4(u_globalLightColor[0], u_globalLightColor[1], u_globalLightColor[2], u_globalLightColor[3]);

    vec3 lightPos = vec3(u_globalLightPos[0], u_globalLightPos[1], u_globalLightPos[2]);
    vec3 lightDir = normalize(vec3(frag_pos) - lightPos);

    float similarity = -1 * dot(lightDir, frag_normal);
    float intensity = min(((similarity + 1.0) * 0.5) * u_shininess, 1);
    vec4 intensityVec = vec4(intensity, intensity, intensity, 1.0);
    final_color = (u_baseColor * diffuse * 0.8 + specular * 0.2) * reflection * lightColor * intensityVec;
}
