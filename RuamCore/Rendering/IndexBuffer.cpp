#include "Renderer.h"
#include "IndexBuffer.h"

namespace RuamEngine
{
    IndexBuffer::IndexBuffer(unsigned int maxCount, unsigned int usage)
    {
        ASSERT(sizeof(unsigned int) == sizeof(GLuint));
        GLCall(glCreateBuffers(1, &m_id));
	    m_maxBytes = maxCount * sizeof(unsigned int);
        setData(nullptr);
	    m_usage = usage;
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_id));
    }

    void IndexBuffer::bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

    void IndexBuffer::unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    // Problema que arreglar. Si se llama a EndBatch cuando se llega al límite del espacio del IndexBuffer
    // y todavía no se puso la información en el Buffer, se va a intentar dibujar con la posible información
    // faltante

    void IndexBuffer::pushBatchData(const std::vector<unsigned int>& data)
    {
        m_indexData.insert(m_indexData.end(), data.begin(), data.end());
        m_currentBytes += data.size() * sizeof(unsigned int);
    }


    // This function does not change the m_indexCount or m_size variables!
    // You probably want to avoid using this function directly
    void IndexBuffer::setSubData(const unsigned int* data, unsigned int offset, unsigned int size)
    {
        ASSERT(offset + size <= maxIndexCount * sizeof(unsigned int));

        bind();

	    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
    }
}
