#pragma once

#include "RendererCore.h"
#include "RuamUtils.h"
#include "GLFW/glfw3.h"

// Think of rows when thinking about every vertex array element.
// Every row contains how to read the buffer.

// One VertexBufferElement defines how to read and create ONE of the multiple components of a vertex
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			 return 4;
			case GL_UNSIGNED_INT:	 return 4;
			case GL_UNSIGNED_BYTE:	 return 1;
		}
		ASSERT(false);
		return 0;
	}

	VertexBufferElement(const unsigned int i_type, const unsigned int i_count, const bool i_normalized)
		: type(i_type), count(i_count),	normalized(i_normalized)
	{

	}
};


// One VertexBufferLayout object Layout defines how to read one buffer to draw multiple vertexes.
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;

	//  The stride is the amount of bytes between each vertex in a vertex buffer
	//  E.G: 
	//  [
	//		X   Y   Z   R    G    B
	//		1,  2,  3,  255, 255, 255, 
	//		4,  5,  6,  254, 254, 254
	//  ]
	// There, the stride would be 6 * 4 bytes, since we have 6 floats, and each float is 4 bytes.
	unsigned int m_Stride;
public:

	VertexBufferLayout()
		: m_Stride(0) {}


	void Reset()
	{
		m_Stride = 0;
		m_Elements.clear();
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(sizeof(T) == 0, "Unsupported Type!");
		//std::runtime	_error();
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back( { GL_FLOAT, count, GL_FALSE } );
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT)*count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE , count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE)*count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};

using VertexBufferLayoutPtr = std::shared_ptr<VertexBufferLayout>;
