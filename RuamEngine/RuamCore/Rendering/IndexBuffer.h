#pragma once

#include "RendererCore.h"

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	//unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void SetData(unsigned int size, unsigned int* data, GLenum usage);
	void SetSubData(unsigned int offset, unsigned int size, unsigned int* data);
	//unsigned int GetCount() const { return m_count; }
};

using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
