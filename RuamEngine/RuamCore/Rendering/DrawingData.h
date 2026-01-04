#pragma once
#include "RenderingCore.h"
#include "RenderingElements.h"
#include "DrawingData.h"
#include "Renderer.h"
#include "Material.h"
#include "RenderUnit.h"

namespace RuamEngine
{
    class Material;
    class RenderUnit;
    class Shader;
    class DrawingData
    {
    public:
        DrawingData();
        ~DrawingData();
        ShaderProgramSPtr m_program = nullptr;
        std::map<unsigned int, RenderUnitSPtr> m_renderUnits = {}; // The key is the material id

        void submitData();
        void submitData(RenderUnit& renderUnit);
		void flush();

		unsigned int instanceId() const { return m_instanceId; }
    private:
		unsigned int m_instanceId;
		static unsigned int s_instanceIdCount;
    };
    using DrawingDataSPtr = std::shared_ptr<DrawingData>;
    using DrawingDataWPtr = std::weak_ptr<DrawingData>;

}
