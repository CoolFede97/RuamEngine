#pragma once

#include "RendererCore.h"

class IndexBuffer
{
	class IndexBuffer
	{
	private:
		unsigned int m_id;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_STATIC_DRAW;
		std::vector<unsigned int> m_indexData = {};

	void Bind() const;
	void Unbind() const;
	void SetData(unsigned int size, unsigned int* data, GLenum usage);
	void SetSubData(unsigned int offset, unsigned int size, unsigned int* data);
	//unsigned int GetCount() const { return m_count; }
};

		void Bind() const;
		void Unbind() const;
		void AddBatchData(const std::vector<unsigned int> data);
		void SetSubData(const unsigned int* data, unsigned int offset, unsigned int size);
		void SetData(const unsigned int* data);

		// Puts the data from m_indexData into the actual buffer
		void SubmitData();
	
		void Flush();
		unsigned int GetIndexCount() const { return m_currentBytes/sizeof(unsigned int); }
		unsigned int GetCurrentSize() const { return m_currentBytes; }
		unsigned int GetMaxSize() const { return m_maxBytes; }
	};

	using IndexBufferPtr = std::unique_ptr<IndexBuffer>;
}
