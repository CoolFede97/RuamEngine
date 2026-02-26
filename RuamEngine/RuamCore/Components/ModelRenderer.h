#pragma once

#include "Renderer.h"
#include "RenderUnit.h"
#include "Component.h"
#include <string>
#include <vector>
#include "Model.h"


namespace RuamEngine
{
    class Vertex;

    #define MODEL_RENDERER_SERIALIZED_MEMBERS(X, ...) \
    	X(m_meshPath, std::string, "", [this]()->void{ this ->LoadModel();})

    class ModelRenderer : public Component
    {
    	using Component::Component;

    private:
    	std::vector<Vertex> m_vertices;
    	std::vector<unsigned int> m_indices;

    	void update() override;

    protected:
    	MODEL_RENDERER_SERIALIZED_MEMBERS(DECL_MEMBER)

    public:
    	ModelWPtr m_model;
    	ShaderProgramType m_shaderProgramType;
    	std::unordered_map<unsigned int, RenderUnitWPtr> m_cachedRenderUnits;

    	std::string name() override { return "ModelRenderer"; }
    	IMPL_DRAW_SERIALIZED_MEMBERS(MODEL_RENDERER_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
    	IMPL_SERIALIZE(ModelRenderer, MODEL_RENDERER_SERIALIZED_MEMBERS(SER_FIELD, ,))

    	~ModelRenderer();

    	void setModel(const std::string& relativePath);
    	void LoadModel();
    };
}
