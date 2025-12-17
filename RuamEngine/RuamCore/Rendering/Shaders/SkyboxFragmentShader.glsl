#version 450 core

out vec4 frag_color;

in vec3 frag_texCoords;

uniform samplerCube skybox;

void main()
{
    frag_color = texture(skybox, frag_texCoords);
}
