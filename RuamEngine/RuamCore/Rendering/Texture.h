#pragma once

#include "RenderingCore.h"
#include <memory>
#include <string>

namespace RuamEngine
{
    class Texture
    {
    protected:
        std::string m_filePath;
        GLuint m_rendererId = 0;
        unsigned int m_rendererIndex = 0; // Index in the handle vector
        GLuint64 m_handle = 0;

    public:
        virtual ~Texture() = default;

        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual void Unbind() const = 0;

        virtual GLuint GetRendererIndex() const { return m_rendererIndex; }
        virtual GLuint GetRendererId() const { return m_rendererId; }
        virtual GLenum GetType() const = 0;

        virtual void SetRendererIndex(unsigned int index) {m_rendererIndex = index;}

        virtual std::string GetPath() const { return m_filePath; }
        GLuint64 GetHandle() const { return m_handle; }

    };

    using TextureSPtr = std::shared_ptr<Texture>;
    using TextureWPtr = std::weak_ptr<Texture>;
}
