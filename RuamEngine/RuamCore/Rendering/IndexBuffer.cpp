#include "Renderer.h"
#include "IndexBuffer.h"

namespace RuamEngine
{
    IndexBuffer::IndexBuffer(unsigned int maxCount, unsigned int usage)
    {
        ASSERT(sizeof(unsigned int) == sizeof(GLuint));
        GLCall(glGenBuffers(1, &m_RendererID));
	    m_maxSize = maxCount * sizeof(unsigned int);
	    m_usage = usage;
	    SetData(0);
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

    // Problema que arreglar. Si se llama a EndBatch cuando se llega al límite del espacio del IndexBuffer
    // y todavía no se puso la información en el Buffer, se va a intentar dibujar con la posible información
    // faltante

    void IndexBuffer::AddBatchData(const std::vector<unsigned int> data, unsigned int size)
    {
        m_indexData.insert(m_indexData.end(), data.begin(), data.end());
        m_currentSize += size;
    }


    // This function does not change the m_indexCount or m_size variables!
    // You probably want to avoid using this function directly
    void IndexBuffer::SetSubData(const unsigned int* data, unsigned int offset, unsigned int size)
    {
        ASSERT(offset + size <= maxIndexCount * sizeof(unsigned int));

        Bind();

	    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
    }

    // This function does not change the m_indexCount or m_size variables!
    // You probably want to avoid using this function directly
    void IndexBuffer::SetData(const unsigned int* data)
    {
        Bind();
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(unsigned int), data, m_usage));
    }

    // Puts the data from m_indexData into the actual buffer
    void IndexBuffer::SubmitData()
    {
        Bind();
	    //std::cout << "INDEX Vector size: " << m_indexData.size() * sizeof(m_indexData[0]) << "\n";
	    //std::cout << "INDEX Buffer size: " << m_currentSize << "\n";
     //   std::cout << "Indices number: " << m_indexData.size() << "\n";
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(unsigned int), m_indexData.data(), m_usage));
    }

    void IndexBuffer::Flush()
    {


	    m_indexData.clear();
        m_currentSize = 0;
        Bind();
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, m_usage));
    }
}