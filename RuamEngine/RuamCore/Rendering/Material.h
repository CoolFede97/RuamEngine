#pragma once
#include "Texture.h"
#include "Vec4.h"
#include "Texture2D.h"
#include <unordered_map>
namespace RuamEngine
{
    class Material
    {
    public:

        Material();
        Vec4 baseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // blanco por defecto
        float m_mainTextureIndex = 0.0f;
        float m_diffuseIndex = 0.0f;
        float m_specularIndex = 0.0f;
        float m_reflectionIndex = 0.0f;
        float m_shininess = 1.0f;
        float m_cubemap = 0.0f;

        TextureWPtr m_diffuseTexture = {};
        TextureWPtr m_specularTexture = {};
        TextureWPtr m_reflectionTexture = {};

	    unsigned int GetId() const { return m_id; }
    private:
        unsigned int m_id;
		static unsigned int s_idCount;
    };
    using MaterialSPtr = std::shared_ptr<Material>;
    using MaterialWPtr = std::weak_ptr<Material>;
}
