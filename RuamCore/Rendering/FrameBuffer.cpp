#include "FrameBuffer.h"
#include "RenderingCore.h"

namespace RuamEngine
{
    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_glName);
        glDeleteTextures(1, &m_texture);
        glDeleteRenderbuffers(1, &m_renderBuffer);
        std::cout << "Frame buffer destroyed!\n";
    }
    FrameBuffer::FrameBuffer(int width, int height)
    {
        generate(width, height);
    }
    void FrameBuffer::generate(int width, int height)
    {
        m_width = width;
        m_height = height;

        glCreateFramebuffers(1, &m_glName);
        glBindFramebuffer(GL_FRAMEBUFFER, m_glName);

        // Create texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);


        // Create render buffer
        glCreateRenderbuffers(1, &m_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

        checkStatus();
    }
    void FrameBuffer::rescale(int width, int height)
    {
        if (m_width == width && m_height == height)
        {
            std::cout << "Warning: Tried to rescale a frame buffer, but it already has the size inserted\n";
            return;
        }
        glDeleteFramebuffers(1, &m_glName);
        glDeleteTextures(1, &m_texture);
        glDeleteRenderbuffers(1, &m_renderBuffer);

        generate(width, height);
    }
    void FrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_glName);
        glViewport(0, 0, m_width, m_height);
    }
    void FrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void FrameBuffer::attachRenderBuffer(unsigned int bufferType, unsigned int renderBuffer)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, bufferType, GL_FRAMEBUFFER, renderBuffer);
    }
    void FrameBuffer::renderBufferStorage(unsigned int renderBufferType, int width, int height)
    {
        glRenderbufferStorage(GL_FRAMEBUFFER, renderBufferType, width, height);
    }
    bool FrameBuffer::checkStatus()
    {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Error: Frame buffer status check failed!\n";
            return false;
        }
        return true;
    }
}
