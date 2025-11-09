#pragma once

#include "RenderingCore.h"
#include "Vertex.h"

namespace RuamEngine
{	
	template<typename T>
	class SSBO
	{
	private:
		unsigned int m_id;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_DYNAMIC_STORAGE_BIT;
	public:
		std::vector<T> m_data = {};

		SSBO(unsigned int maxCount, unsigned int usage)
		{
			GLCall(glCreateBuffers(1, &m_id));
			GLCall(
				glNamedBufferStorage
				(
					m_id,
					sizeof(T) * maxCount,
					0,
					usage
				));
			m_maxBytes = maxCount * sizeof(T);
			m_usage = usage;
		}
		~SSBO()
		{
			GLCall(glDeleteBuffers(1, &m_id));
		}

		// Should be used for buffers from the renderer batch
		void AddBatchData(const std::vector<T>& data)
		{
			m_data.insert(m_data.end(), data.begin(), data.end());
			m_currentBytes += data.size() * sizeof(T);
		}

		// Shouldn't be used when using batch rendering
		void SubmitData()
		{
			ASSERT(m_data.size() * sizeof(T) <= m_maxBytes);
			GLCall(glNamedBufferSubData(m_id, 0, m_data.size()*sizeof(T), m_data.data()));
		}
	
		// Puts the data from m_data into the actual SSBO
		void BindBufferBase(const int& binding)
		{
			GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_id));
		}

		void Flush()
		{
			m_data.clear();
			m_currentBytes = 0;
		}

		unsigned int GetID() const { return m_id; }
		unsigned int GetCurrentSize() const { return m_currentBytes; }
		unsigned int GetMaxSize() const { return m_maxBytes; }

	};
	template<typename T>
	using SSBOPointer = std::unique_ptr<SSBO<T>>;
}

