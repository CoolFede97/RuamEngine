#pragma once

#include "RenderingCore.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace RuamEngine
{
	class VertexArray
	{
	private:
		unsigned int m_RendererID;
	public:

	
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
	};

	using VertexArrayPtr = std::unique_ptr<VertexArray>;
}

