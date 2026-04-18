#pragma once

#include "Texture2D.h"
#include "Vec2.h"
namespace RuamEngine
{
    struct CameraMatrices
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };
    class FrameBuffer
    {
        unsigned int m_glName;
        unsigned int m_texture;
        unsigned int m_renderBuffer;
        CameraMatrices m_cameraMatrices;
        int m_width = 0;
        int m_height = 0;

        void generate(int width, int height);
    public:
        ~FrameBuffer();
        FrameBuffer(int width, int height);

        void bind();
        void unbind();

        void attachRenderBuffer(unsigned int bufferType, unsigned int renderBuffer);
        void renderBufferStorage(unsigned int renderBufferType, int width, int height);

        bool checkStatus();

        void rescale(int width, int height);

        inline int width() const { return m_width; }
        inline int height() const { return m_height; }
        inline unsigned int texture() const { return m_texture; }
    };
    using FrameBufferSPtr = std::shared_ptr<FrameBuffer>;
}
