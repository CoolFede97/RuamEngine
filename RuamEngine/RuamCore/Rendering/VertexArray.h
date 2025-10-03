#pragma once

#include "RenderingCore.h"
#include "Buffer.h"
#include "VertexBufferLayout.h"

namespace RuamEngine
{
	class VertexArray
	{
	private:
		unsigned int m_id;
	public:

	
		VertexArray();
		~VertexArray();

		//void AddBuffer(const SSBO<Vertex>& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
	};

	using VertexArrayPtr = std::unique_ptr<VertexArray>;
}

