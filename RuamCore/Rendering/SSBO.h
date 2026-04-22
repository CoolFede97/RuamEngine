#pragma once

#include "RenderingCore.h"
#include <vector>
#include <memory>

namespace RuamEngine
{
	enum SSBOType
    {
        vertices = 0,
        indices = 1,
        modelMatrices = 2,
        textures2D = 3,
        cubemaps = 4
    };

    template<typename T>
	class SSBO
	{
	private:
		unsigned int m_glName;
		unsigned int m_maxBytes = 0;
		unsigned int m_currentBytes = 0;
		unsigned int m_usage = GL_DYNAMIC_STORAGE_BIT;
	public:
		std::vector<T> m_data = {};

		SSBO(unsigned int maxCount, unsigned int usage)
		{
			GLCall(glCreateBuffers(1, &m_glName));
			GLCall(
				glNamedBufferStorage
				(
					m_glName,
					sizeof(T) * maxCount,
					0,
					usage
				));
			m_maxBytes = maxCount * sizeof(T);
			m_usage = usage;
		}
		~SSBO()
		{
			GLCall(glDeleteBuffers(1, &m_glName));
		}

		// Should be used for buffers from the renderer batch
		void addBatchData(const std::vector<T>& data)
		{
			m_data.insert(m_data.end(), data.begin(), data.end());
			m_currentBytes += data.size() * sizeof(T);
		}

		// Shouldn't be used when using batch rendering
		void submitData()
		{
			ASSERT(m_data.size() * sizeof(T) <= m_maxBytes);
			GLCall(glNamedBufferSubData(m_glName, 0, m_data.size()*sizeof(T), m_data.data()));
		}

		// Puts the data from m_data into the actual SSBO
		void bindBufferBase(const int& binding)
		{
			GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_glName));
		}

		void flush()
		{
			m_data.clear();
			m_currentBytes = 0;
		}

		bool checkIfEnoughSpace(unsigned int elementsToAddAmount)
		{
		    if (currentSize() + elementsToAddAmount * sizeof(T) > maxSize()) return false;
			return true;
		}
		unsigned int glName() const { return m_glName; }
		unsigned int currentSize() const { return m_currentBytes; }
		unsigned int maxSize() const { return m_maxBytes; }

	};
	template<typename T>
	using SSBOUPtr = std::unique_ptr<SSBO<T>>;
}
