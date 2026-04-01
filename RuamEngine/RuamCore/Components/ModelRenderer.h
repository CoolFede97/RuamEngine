#pragma once

#include "ShaderProgram.h"
#include "RenderUnit.h"
#include "Component.h"
#include <string>
#include <vector>
#include "Model.h"


namespace RuamEngine
{
    class Vertex;

    #define MODEL_RENDERER_SERIALIZED_MEMBERS(X, ...) \
    	X(m_meshPath, std::string, "", [this]()->void{ this ->loadModel();})

    class ModelRenderer : public Component
    {
    	using Component::Component;

    private:
    	float m_vertices = 0;
    	float m_indices = 0;

     	void renderUpdate() override;

      protected:
    	MODEL_RENDERER_SERIALIZED_MEMBERS(DECL_MEMBER)

    public:
	   	ModelRenderer(Json modelRendererData, const unsigned int entityId);
    	ModelWPtr m_model;
    	ShaderProgramType m_shaderProgramType = ShaderProgramType::general;
    	std::unordered_map<unsigned int, RenderUnitWPtr> m_cachedRenderUnits;

    	std::string name() override { return "ModelRenderer"; }
    	IMPL_DRAW_SERIALIZED_MEMBERS(MODEL_RENDERER_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
    	IMPL_SERIALIZE(ModelRenderer, MODEL_RENDERER_SERIALIZED_MEMBERS(SER_FIELD, ,))

    	~ModelRenderer();

    	void setModel(const std::string& relativePath);
    	void loadModel();
    };
}
