#version 450 core

const int SSBOType_vertices = 0;
const int SSBOType_indices = 1;

layout(binding = SSBOType_vertices, std430) readonly buffer ssbo0
{
    float positions[];
};

layout(binding = SSBOType_indices, std430) readonly buffer ssbo1
{
    uint indices[];
};

out vec3 frag_texCoords;

uniform mat4 view;
uniform mat4 projection;

vec3 GetPosition(uint index)
{
    return vec3(positions[index * 3], positions[index * 3 + 1], positions[index * 3 + 2]);
}

void main()
{
    uint realIndex = indices[gl_VertexID];
    frag_texCoords = GetPosition(realIndex);
    gl_Position = projection * view * vec4(frag_texCoords, 1.0);
}
