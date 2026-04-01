#include "Cubemap.h"
#include "DebugUtils.h"
#include "FileFunctions.h"
#include "OpenGLUtils.h"
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include "RenderingCore.h"

namespace RuamEngine
{

    Cubemap::Cubemap(const std::vector<std::string>& relativePaths)
    : m_localBuffers({})
    {
        m_filePaths = GlobalizePaths(relativePaths);

        std::string unifiedPath = UnifyPaths(relativePaths);
        m_filePath = unifiedPath;

        GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_glName));

        stbi_set_flip_vertically_on_load(1);

        for (unsigned int i = 0; i < relativePaths.size(); i++)
        {
            m_localBuffers.push_back(stbi_load(m_filePaths[i].c_str(), &m_faceLength, &m_faceLength, &m_BPP, 4));

            if (m_localBuffers[i] == nullptr)
            {
                const char* reason = stbi_failure_reason();
    			if (reason) {
    				std::cerr << "Couldn't load cubemap texture at path : "<< m_filePaths[i] << "\n The reason was: " << reason << "\n";
    			}
    			else {
    				std::cerr << "Couldn't load cubemap texture at path :" << m_filePaths[i] << "\n There was no apparent reason\n";
    			}
            }
            else
            {
                std::cout << "Cubemap texture at path: " << m_filePaths[i] << " was loaded succesfully" << "\n";
            }

            ASSERT(m_localBuffers[i]);
        }

        GLCall(glTextureStorage2D(m_glName, 1, GL_RGBA8, m_faceLength, m_faceLength));

        for (unsigned int i = 0; i < relativePaths.size(); i++)
        {
            GLCall(glTextureSubImage3D(m_glName, 0, 0, 0, i, m_faceLength, m_faceLength, 1, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffers[i]));
            stbi_image_free(m_localBuffers[i]);
        }

        glTexParameteri(m_glName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_glName, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    Cubemap::Cubemap(const std::string& relativePath)
    {
        m_filePaths.push_back(GlobalizePath(relativePath));
        m_filePath = m_filePaths[0];

        GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_glName));

        stbi_set_flip_vertically_on_load(1);

        int fullWidth = 0;
        int fullHeight = 0;

        m_localBuffers.push_back(stbi_load(m_filePath.c_str(), &fullWidth, &fullHeight, &m_BPP, 4));

        if (m_localBuffers[0] == nullptr)
        {
            const char* reason = stbi_failure_reason();
 			if (reason) {
				std::cerr << "Couldn't load cubemap texture at path : "<< m_filePath << "\n The reason was: " << reason << "\n";
 			}
 			else {
				std::cerr << "Couldn't load cubemap texture at path :" << m_filePath << "\n There was no apparent reason\n";
 			}
        }
        else
        {
            std::cout << "Cubemap texture at path: " << m_filePath << " was loaded succesfully" << "\n";
        }

        ASSERT(m_localBuffers[0]);

        // This assumes that the image provided contains the faces in a "Jesus way"

        int faceWidth = fullWidth/4;
        int faceHeight = fullHeight/3;
        ASSERT(faceWidth == faceHeight);

        m_faceLength = faceWidth;

        GLCall(glTextureStorage2D(m_glName, 1, GL_RGBA8, m_faceLength, m_faceLength));

        struct FacePosition
        {
            int col, row;
        };

        FacePosition facePositions[6] =
        {
            {2, 1},  // 0: +X (right)
            {0, 1},  // 1: -X (left)
            {1, 0},  // 2: +Y (top)
            {1, 2},  // 3: -Y (bottom)
            {1, 1},  // 4: +Z (front)
            {3, 1}   // 5: -Z (back)
        };

        int channels = 4; // RGBA

        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char*  faceData = new unsigned char[m_faceLength*m_faceLength*channels];

            int faceStartX = facePositions[i].col * m_faceLength;
            int faceStartY = facePositions[i].row * m_faceLength;

            for (int y = 0; y < m_faceLength; y++)
            {
                for (int x = 0; x < m_faceLength; x++)
                {
                    int srcX = faceStartX + x;
                    int srcY = faceStartY + y;
                    int srcIndex = (srcY * fullWidth + srcX) * channels;

                    int dstIndex = (y * m_faceLength + x) * channels;

                    faceData[dstIndex + 0] = m_localBuffers[0][srcIndex + 0];
                    faceData[dstIndex + 1] = m_localBuffers[0][srcIndex + 1];
                    faceData[dstIndex + 2] = m_localBuffers[0][srcIndex + 2];
                    faceData[dstIndex + 3] = m_localBuffers[0][srcIndex + 3];

                }
            }

            GLCall(glTextureSubImage3D(m_glName, 0, 0, 0, i, m_faceLength, m_faceLength, 1, GL_RGBA, GL_UNSIGNED_BYTE, faceData));

            delete[] faceData;
        }

        stbi_image_free(m_localBuffers[0]);

        glTexParameteri(m_glName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_glName, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_glName, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Cubemap::bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_glName));
    }
    void Cubemap::unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }
    Cubemap::~Cubemap()
    {
        GLCall(glDeleteTextures(1, &m_glName));
    }
}
