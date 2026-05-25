#pragma once

#include "MeshRU.h"

#include <iostream>
#include <vector>
namespace RuamEngine
{
    struct ModelRU
    {
        std::vector<MeshRUSPtr> m_meshRUs;
        void setInstanceId(unsigned int id);
        const inline unsigned int instanceId() const { return m_instanceId; }

    private:
        unsigned int m_instanceId;
    };

    using ModelRUSPtr = std::shared_ptr<ModelRU>;
    using ModelRUWPtr = std::weak_ptr<ModelRU>;
}
