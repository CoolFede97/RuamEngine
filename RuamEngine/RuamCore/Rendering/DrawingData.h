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
        ShaderPtr m_shader = nullptr;
        std::unordered_map<unsigned int, RenderUnit> m_renderUnits = {};

        void SubmitBatchData();
        void SubmitBatchData(RenderUnit& renderUnit);
		void Flush();
    private:
		unsigned int m_instanceId;
		static unsigned int s_instanceIdCount;
    };
}
