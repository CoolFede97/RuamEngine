#pragma once

#include "RenderingCore.h"
#include "Vertex.h"

namespace RuamEngine
{	
	class VertexBuffer
	{
	private:
		unsigned int m_id;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_STATIC_DRAW;
	public:
		std::vector<float> m_vertexData = {};

		VertexBuffer(unsigned int maxSize, unsigned int usage);
		~VertexBuffer();

		// Should be used for buffers from the renderer batch
		void AddBatchData(const std::vector<float> data, unsigned int size);

		// Shouldn't be used when using batch rendering
		void SetSubData(const void* data, unsigned int offset, unsigned int size);
	
		// Shouldn't be used when using batch rendering
		void SetData(const void* data);
	
		// Puts the data from m_vertexData into the actual buffer
		void SubmitData();

		void Flush();

		void Bind() const; 
		void Unbind() const;
		unsigned int GetID() const { return m_id; }
		unsigned int GetCurrentSize() const { return m_currentBytes; }
		unsigned int GetMaxSize() const { return m_maxBytes; }

	};

	using VertexBufferPtr = std::unique_ptr<VertexBuffer>;
}

