#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "RenderingConstants.h"
namespace RuamEngine
{
    VertexBuffer::VertexBuffer(unsigned int maxSize, unsigned int usage)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
	    m_maxSize = maxSize;
	    m_usage = usage;
	    SetData(nullptr);
    }

    VertexBuffer::~VertexBuffer() 
    {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    // Después hacer que si la data supera la capacidad del Buffer (por más que esté vacío), se actualice el buffer y que tire advertencia
    void VertexBuffer::AddBatchData(const std::vector<float> data, unsigned int size)
    {
        m_vertexData.insert(m_vertexData.end(), data.begin(), data.end());
        m_currentSize += size;
    }

    void VertexBuffer::SetSubData(const void* data, unsigned int offset, unsigned int size)
    {
        ASSERT(offset + size <= maxVertexSize * maxVertexCount);

        Bind();

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    }

    void VertexBuffer::SetData(const void* data)
    {
        Bind();
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), data, m_usage));
    }

    // Puts the data from m_vertexData into the actual buffer
    void VertexBuffer::SubmitData()
    {

        Bind();
        
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), m_usage));
	
    }

    void VertexBuffer::Flush()
    {
        m_vertexData.clear();
        m_currentSize = 0;
        Bind();


        GLCall(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, m_usage));
    
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void VertexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}
