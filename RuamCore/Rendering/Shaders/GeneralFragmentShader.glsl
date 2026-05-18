#version 450 core

layout(binding = 0) uniform sampler2D u_diffuse;
layout(binding = 1) uniform sampler2D u_specular;
layout(binding = 2) uniform sampler2D u_reflection;
layout(binding = 3) uniform samplerCube u_cubemap;

out vec4 final_color;

flat in int frag_instance;

smooth in vec4 frag_pos;
smooth in vec3 frag_normal;
smooth in vec2 frag_uv;

uniform vec4 u_baseColor;
uniform float u_shininess;

uniform vec4 u_globalLightColor;
uniform vec3 u_globalLightPos;
uniform float u_lightOffset;

void main()
{
    vec4 diffuse = texture(u_diffuse, frag_uv);
    vec4 specular = texture(u_specular, frag_uv);
    vec4 reflection = texture(u_reflection, frag_uv);

    vec4 lightColor = vec4(u_globalLightColor[0], u_globalLightColor[1], u_globalLightColor[2], u_globalLightColor[3]);

    vec3 lightPos = vec3(u_globalLightPos[0], u_globalLightPos[1], u_globalLightPos[2]);
    vec3 lightDir = normalize(vec3(frag_pos) - lightPos);

    float similarity = -dot(lightDir, frag_normal);
    float intensity = min(((similarity + 1.0) * 0.5) * u_shininess, 1);
    vec4 intensityVec = vec4(intensity, intensity, intensity, 1.0);
    final_color = (u_baseColor * diffuse * 0.8 + specular * 0.2) * reflection * lightColor * intensityVec;
}
