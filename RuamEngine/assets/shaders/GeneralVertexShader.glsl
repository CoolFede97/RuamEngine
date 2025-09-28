#version 330 core

layout(location = 0) in vec4 vertex_pos;
layout(location = 1) in vec2 vertex_texCoords;
layout(location = 2) in float vertex_texId;
 
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 frag_texCoords;
out float frag_texId;

void main()
{
	gl_Position = u_projection * u_view * u_model * vertex_pos;
	frag_texCoords = vertex_texCoords;
	frag_texId = vertex_texId;
};