#pragma once

#include "RenderingCore.h"
#include "SSBO.h"
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
		void bind() const;
		void unbind() const;
	};

	using VertexArrayUPtr = std::unique_ptr<VertexArray>;
}
