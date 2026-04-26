#pragma once

#include "RenderingCore.h"
#include "Vertex.h"
#include <memory>
namespace RuamEngine
{
	class VertexBuffer
	{
	private:
		unsigned int m_glName;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_STATIC_DRAW;
	public:
		std::vector<float> m_vertexData = {};

		VertexBuffer(unsigned int maxSize, unsigned int usage);
		~VertexBuffer();

		// Should be used for buffers from the renderer batch
		void pushBatchData(const std::vector<float>& data, unsigned int size);

		// Shouldn't be used when using batch rendering
		void setSubData(const void* data, unsigned int offset, unsigned int size);

		// Shouldn't be used when using batch rendering
		void setData(const void* data);

		// Puts the data from m_vertexData into the actual buffer
		void submitData();

		void flush();

		void bind() const;
		void unbind() const;
		unsigned int glName() const { return m_glName; }
		unsigned int currentSize() const { return m_currentBytes; }
		unsigned int maxSize() const { return m_maxBytes; }

	};

	using VertexBufferPtr = std::unique_ptr<VertexBuffer>;
}
