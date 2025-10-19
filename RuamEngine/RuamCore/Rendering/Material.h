#pragma once
#include "Vec4.h"
#include "Texture.h"
#include <unordered_map>
namespace RuamEngine
{
    class Material
    {
    public:

        Material();
        Vec4 albedoColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // blanco por defecto
        float m_diffuseHandle = 0.0f;
        float m_metallicHandle = 0.0f;
        float m_roughnessHandle = 1.0f;
        float m_ambientOcclusionHandle = 1.0f;
        float m_emissiveStrengthHandle = 0.0f;

        bool useAlbedoMap = false;
        bool useNormalMap = false;
        bool useMetallicMap = false;
        bool useRoughnessMap = false;
        bool useAOMap = false;
        bool useEmissiveMap = false;

	    unsigned int GetId() const { return m_id; }
    private:
        unsigned int m_id;
		static unsigned int s_idCount;
    };
    using MaterialPtr = std::shared_ptr<Material>;

}