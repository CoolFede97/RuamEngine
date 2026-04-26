#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

namespace RuamEngine
{
	class IndexBuffer
	{
	private:
		unsigned int m_id;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_STATIC_DRAW;
		std::vector<unsigned int> m_indexData = {};

	public:
		IndexBuffer(unsigned int maxCount, unsigned int usage);
		~IndexBuffer();

		void bind() const;
		void unbind() const;
		void pushBatchData(const std::vector<unsigned int>& data);
		void setSubData(const unsigned int* data, unsigned int offset, unsigned int size);
		void setData(const unsigned int* data);

		// Puts the data from m_indexData into the actual buffer
		void submitData();

		void flush();
		unsigned int indexCount() const { return m_currentBytes/sizeof(unsigned int); }
		unsigned int CcrrentSize() const { return m_currentBytes; }
		unsigned int maxSize() const { return m_maxBytes; }
	};

	using IndexBufferPtr = std::unique_ptr<IndexBuffer>;
}
