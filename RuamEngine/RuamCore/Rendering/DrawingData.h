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
    class DrawingData
    {
    public:
        DrawingData(Shader::PipelineType pipelineType);
        DrawingData();
        ~DrawingData();
        ShaderPtr m_shader = nullptr;
        std::unordered_map<Material::MaterialType, RenderUnit> m_renderUnits = {};

        void SubmitBatchData();
        void SubmitBatchData(RenderUnit& renderUnit);
		void Flush();
    private:
		Shader::PipelineType m_pipelineType;
    };
}
