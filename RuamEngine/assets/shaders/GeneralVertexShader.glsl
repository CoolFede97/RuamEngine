#version 450 core

#extension GL_ARB_bindless_texture : require    
#extension GL_ARB_gpu_shader_int64 : enable

const int SSBOType_vertices = 0;
const int SSBOType_indices = 1;
const int SSBOType_modelMatrices = 2;

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

layout(binding = SSBOType_modelMatrices, std430) readonly buffer ssbo2
{
	mat4 modelTransforms[];
};


uniform mat4 u_view;
uniform mat4 u_projection;

vec3 GetPosition(uint index) 
{
    return vec3
    (
        data[index].position[0], 
        data[index].position[1], 
        data[index].position[2]
    );
}

vec2 GetUV(uint index) 
{
    return vec2
    (
        data[index].uv[0], 
        data[index].uv[1]
    );
}

vec3 GetNormal(uint index) 
{
    return vec3
    (
        data[index].normal[0], 
        data[index].normal[1], 
        data[index].normal[2]
    );
}

smooth out vec2 frag_uv;
smooth out vec3 frag_normal;
smooth out vec4 frag_pos;
flat out int frag_instance;

void main()
{
    uint realIndex = indices[gl_VertexID];
    mat4 vp = u_projection * u_view;
    vec4 position = vec4(GetPosition(realIndex), 1.0);
	gl_Position = vp * modelTransforms[gl_InstanceID] * position;
    frag_pos = modelTransforms[gl_InstanceID] * position;
    frag_uv = GetUV(realIndex);
    frag_instance = gl_InstanceID;
    frag_normal = normalize(GetNormal(realIndex));
};