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
        GLuint m_glName = 0;
        unsigned int m_rendererIndex = 0; // Index in the handle vector
        GLuint64 m_handle = 0;

    public:
        virtual ~Texture() = default;

        virtual void bind(unsigned int slot = 0) const = 0;
        virtual void unbind() const = 0;

        virtual GLuint glName() const { return m_glName; }
        virtual GLenum texType() const = 0;

        virtual std::string path() const { return m_filePath; }

    };

    using TextureSPtr = std::shared_ptr<Texture>;
    using TextureWPtr = std::weak_ptr<Texture>;
}
