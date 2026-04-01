#pragma once

#include "RendererCore.h"

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


	void reset()
	{
		m_Stride = 0;
		m_Elements.clear();
	}

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(sizeof(T) == 0, "Unsupported Type!");
		//std::runtime	_error();
	}


	inline const std::vector<VertexBufferElement> elements() const& { return m_Elements; }
	inline unsigned int stride() const { return m_Stride; }
};

using VertexBufferLayoutSPtr = std::shared_ptr<VertexBufferLayout>;
