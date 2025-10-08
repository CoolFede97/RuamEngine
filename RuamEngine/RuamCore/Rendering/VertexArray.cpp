#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	
	// The offset is the amount of bytes from the first component of the vertex to a particular one

	//  E.G: 
	//  [
	//		X   Y   Z   R    G    B
	//		1,  2,  3,  255, 255, 255, 
	//		4,  5,  6,  254, 254, 254
	//  ]
	//  There, the offset of the position component would be 0 because it is the first component of the vertex
	//  The offset of the colour component would be 3 * 4 because there are 3 floats before the colour
	//  component starts in the vertex

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size() ; i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer
		(
			i, 
			element.count,
			element.type,
			element.normalized,
			layout.GetStride(),
			(const void*)offset)
		);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
