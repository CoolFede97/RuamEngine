#pragma once

#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Component.h"
#include <string>
#include <vector>
#include "Model.h"

namespace RuamEngine
{
    class Vertex;

    #define MODEL_RENDERER_SERIALIZED_MEMBERS(X, ...) \
    	X(m_modelPath, std::string, "", [this]()->void{ this ->loadModel();})

    class ModelRenderer : public Component
    {
    	using Component::Component;

    private:
    	float m_vertices = 0;
    	float m_indices = 0;

        ModelSPtr m_model = nullptr;
        SSBOWPtr<glm::mat4> m_matricesSSBO = {};
        ShaderProgramSPtr m_shaderProgram = nullptr;

     	void renderUpdate() override;

      protected:
    	MODEL_RENDERER_SERIALIZED_MEMBERS(DECL_MEMBER)

    public:
	   	ModelRenderer(nlohmann::json modelRendererData, const unsigned int entityId);

    	IMPL_DRAW_SERIALIZED_MEMBERS(MODEL_RENDERER_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
    	IMPL_SERIALIZE(ModelRenderer, MODEL_RENDERER_SERIALIZED_MEMBERS(SER_FIELD, ,))

    	void setModel(const std::string& relativePath);
    	void loadModel();
    private:
        DECL_REGISTER_COMPONENT(ModelRenderer)
    };
}
