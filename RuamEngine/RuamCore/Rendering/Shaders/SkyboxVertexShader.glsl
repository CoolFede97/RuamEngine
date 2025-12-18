#version 450 core

const int SSBOType_vertices = 0;
const int SSBOType_indices = 1;

struct VertexData
{
    float position[3];
    float uv[2];
    float normal[3];
};

layout(binding = SSBOType_vertices, std430) readonly buffer ssbo0
{
    VertexData data[];
};

layout(binding = SSBOType_indices, std430) readonly buffer ssbo1
{
    uint indices[];
};

out vec3 frag_texCoords;

uniform mat4 u_view;
uniform mat4 u_projection;

vec3 GetPosition(uint index)
{
    return vec3(
        data[index].position[0],
        data[index].position[1],
        data[index].position[2]
    );
}

void main()
{
    uint realIndex = indices[gl_VertexID];
    frag_texCoords = GetPosition(realIndex);

    mat4 viewNoTranslation = mat4(mat3(u_view));

    vec4 pos = u_projection * viewNoTranslation * vec4(frag_texCoords, 1.0);
    gl_Position = pos.xyww;
}
