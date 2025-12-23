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
        ShaderProgramPtr m_program = nullptr;
        std::map<unsigned int, RenderUnitPtr> m_renderUnits = {};

        void SubmitData();
        void SubmitData(RenderUnit& renderUnit);
		void Flush();

		unsigned int GetInstanceId() const { return m_instanceId; }
    private:
		unsigned int m_instanceId;
		static unsigned int s_instanceIdCount;
    };
    using DrawingDataPtr = std::shared_ptr<DrawingData>;

}
