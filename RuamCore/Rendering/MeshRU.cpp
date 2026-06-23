#include "MeshRU.h"

namespace RuamEngine
{
	void MeshRU::submitData()
	{
		if (m_vertices->currentSize() > 0)
		{
			if (!m_uploaded)
			{
				m_vertices->submitData();
			}
		}
		if (m_indices->currentSize() > 0)
		{
			if (!m_uploaded)
			{
				m_indices->submitData();
			}
		}

		if (!m_uploaded && (m_vertices->currentSize() > 0 || m_indices->currentSize() > 0))
		{
			m_uploaded = true;
		}
	}

	void MeshRU::bindBuffersBase()
	{
		m_vertices->bindBufferBase(SSBOType::vertices);
		m_indices->bindBufferBase(SSBOType::indices);
	}

	void MeshRU::pushData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
	{
		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += vertices.size();

		m_vertices->pushData(vertices);

        m_indices->pushData(indices);
	}

	MeshRU::MeshRU(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
	    pushData(vertices, indices);
		submitData();
	}
}
