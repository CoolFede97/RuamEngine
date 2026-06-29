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

    class ModelRenderer : public Component
    {
    	using Component::Component;

    private:
    	float m_vertices = 0;
    	float m_indices = 0;
        std::string m_modelPath = "";
        ModelSPtr m_model = nullptr;
        SSBOWPtr<glm::mat4> m_matricesSSBO = {};
        ShaderProgramSPtr m_shaderProgram = nullptr;

     	void renderUpdate() override;

    public:
   		std::vector<FieldInfo> fields() override
		{
		    return
       		{
                makeFieldInfo<std::string>("m_modelPath", m_modelPath, [this]()->void{ this->loadModel();})
            };
		}
		void renderStart();
    	void setModel(const std::string& relativePath);
    	void loadModel();
    private:
        DECL_REGISTER_COMPONENT(ModelRenderer)
    };
}
