#version 450 core

layout(binding = 3) uniform samplerCube u_cubemap;

out vec4 frag_color;

in vec3 frag_texCoords;

void main()
{
    frag_color = texture(u_cubemap, frag_texCoords);
}
