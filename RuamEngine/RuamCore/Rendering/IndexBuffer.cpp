#include "Renderer.h"
#include "IndexBuffer.h"

using namespace RuamEngine;

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::SetData(unsigned int count, unsigned int* data, GLenum usage)
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage));
}

void IndexBuffer::SetSubData(unsigned int offset, unsigned int size, unsigned int* data)
{
    ASSERT(offset + size <= maxIndexCount * sizeof(unsigned int));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
}

