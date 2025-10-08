#include "Renderer.h"
#include "IndexBuffer.h"

using namespace RuamEngine;

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    IndexBuffer::IndexBuffer(unsigned int maxCount, unsigned int usage)
    {
        ASSERT(sizeof(unsigned int) == sizeof(GLuint));
        GLCall(glCreateBuffers(1, &m_id));
	    m_maxBytes = maxCount * sizeof(unsigned int);
        SetData(nullptr);
	    m_usage = usage;
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

    void IndexBuffer::AddBatchData(const std::vector<unsigned int> data)
    {
        m_indexData.insert(m_indexData.end(), data.begin(), data.end());
        m_currentBytes += data.size() * sizeof(unsigned int);
    }

void IndexBuffer::SetSubData(unsigned int offset, unsigned int size, unsigned int* data)
{
    ASSERT(offset + size <= maxIndexCount * sizeof(unsigned int));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
}

	    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
    }

    // This function does not change the m_indexCount or m_size variables!
    // You probably want to avoid using this function directly
    void IndexBuffer::SetData(const unsigned int* data)
    {
        Bind();
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxBytes, data, m_usage));
    }

    // Puts the data from m_indexData into the actual buffer
    void IndexBuffer::SubmitData()
    {
        Bind();
	    //std::cout << "INDEX Vector size: " << m_indexData.size() * sizeof(m_indexData[0]) << "\n";
	    //std::cout << "INDEX Buffer size: " << m_currentBytes << "\n";
     //   std::cout << "Indices number: " << m_indexData.size() << "\n";
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(unsigned int), m_indexData.data(), m_usage));
    }

    void IndexBuffer::Flush()
    {
	    m_indexData.clear();
        m_currentBytes = 0;
        Bind();
        std::vector<unsigned int> zeros(m_maxBytes / sizeof(unsigned int), 0);
        GLCall(glNamedBufferSubData(m_id, 0, m_maxBytes, zeros.data()));
    }
}
