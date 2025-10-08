#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

using namespace RuamEngine;

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::SetSubData(unsigned int offset, unsigned int size, const void* data)
{
    ASSERT(offset + size <= maxVertexSize * maxVertexCount);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::SetData(const void* data, unsigned int size, GLenum usage)
{
    Renderer::m_state.m_layout->Reset();
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}


void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
