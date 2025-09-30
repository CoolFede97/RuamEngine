#pragma once
#include "Vec4.h"
#include "Texture.h"
#include <unordered_map>
namespace RuamEngine
{
    class Material
    {
    public:
        enum MaterialType
        {
		    Generic = 0
	    };

        Material(MaterialType materalType);
        Vec4 albedoColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // blanco por defecto
        float metallic = 0.0f;
        float roughness = 1.0f;
        float ambientOcclusion = 1.0f;
        float emissiveStrength = 0.0f;

        std::vector<std::unique_ptr<Texture>> textures;

        bool useAlbedoMap = false;
        bool useNormalMap = false;
        bool useMetallicMap = false;
        bool useRoughnessMap = false;
        bool useAOMap = false;
        bool useEmissiveMap = false;

	    unsigned int GetID() const { return m_type; }
    private:
        MaterialType m_type;
    };
    using MaterialPtr = std::unique_ptr<Material>;

}