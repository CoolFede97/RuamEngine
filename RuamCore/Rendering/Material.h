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
        ~Material();
        Vec4 baseColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // blanco por defecto

        float m_shininess = 1.0f;

        TextureSPtr m_diffuseTexture = {};
        TextureSPtr m_specularTexture = {};
        TextureSPtr m_reflectionTexture = {};

	    unsigned int id() const { return m_id; } // Is the instance id
    private:
        unsigned int m_id;
		static unsigned int s_idCount;
    };
    using MaterialSPtr = std::shared_ptr<Material>;
    using MaterialWPtr = std::weak_ptr<Material>;
}
