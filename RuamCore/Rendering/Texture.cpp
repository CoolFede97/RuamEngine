#include "Texture.h"
#include "ResourceManager.h"

namespace RuamEngine
{
    Texture::~Texture()
    {
        ResourceManager::RemoveTextureIfExpired(m_filePath);
    }
}
