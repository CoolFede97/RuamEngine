#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "RenderingConstants.h"
namespace RuamEngine
{
    VertexBuffer::VertexBuffer(unsigned int maxSize, unsigned int usage)
    {
        GLCall(glGenBuffers(1, &m_glName));
	    m_maxBytes = maxSize;
	    m_usage = usage;
	    setData(nullptr);
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_glName));
    }

    // Después hacer que si la data supera la capacidad del Buffer (por más que esté vacío), se actualice el buffer y que tire advertencia
    void VertexBuffer::pushBatchData(const std::vector<float>& data, unsigned int size)
    {
        m_vertexData.insert(m_vertexData.end(), data.begin(), data.end());
        m_currentBytes += size;
    }

    void VertexBuffer::setSubData(const void* data, unsigned int offset, unsigned int size)
    {
        ASSERT(offset + size <= maxVertexSize * maxVertexCount);

        bind();

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    }

    void VertexBuffer::setData(const void* data)
    {
        bind();
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), data, m_usage));
    }

    // Puts the data from m_vertexData into the actual buffer
    void VertexBuffer::submitData()
    {

        bind();

        GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), m_usage));

    }
}
