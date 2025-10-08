#pragma once

#include "RendererCore.h"

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void SetSubData(unsigned int offset, unsigned int size, const void* data);
	void SetData(const void* data, unsigned int size, GLenum usage);

	void Bind() const; 
	void Unbind() const;
	unsigned int GetID() const { return m_RendererID; }
};

using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
