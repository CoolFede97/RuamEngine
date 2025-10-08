#version 330 core

out vec4 final_color;

in vec4 frag_color;

void main()
{
    final_color = frag_color;
};