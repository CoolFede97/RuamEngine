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

	void MeshRU::pushBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
	{
		if (m_vertices->checkIfPushIsBiggerThanMaxSize(vertices.size())) resizeSSBO(m_vertices, true, vertices.size());
		if (m_indices->checkIfPushIsBiggerThanMaxSize(indices.size())) resizeSSBO(m_indices, true, indices.size());

		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += vertices.size();

		if (!m_vertices->checkIfEnoughSpaceForPush(vertices.size())) resizeSSBO(m_vertices, false, m_vertices->maxElements());
		m_vertices->pushBatchData(vertices);

		if (!m_indices->checkIfEnoughSpaceForPush(indices.size())) resizeSSBO(m_indices, false, m_indices->maxElements());
        m_indices->pushBatchData(indices);
	}

	MeshRU::MeshRU(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
	    pushBatchData(vertices, indices);
		submitData();
	}
}
