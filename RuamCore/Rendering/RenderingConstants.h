#pragma once
#include "glm/glm.hpp"

namespace RuamEngine
{
	static const unsigned int maxVertexAtribs = (3 + 4 + 2 + 1); // Position, Color, TexCoords, TexID
	static const unsigned int maxVertexSize = sizeof(float) * maxVertexAtribs; // 3 position, 4 color, 2 tex coords, 1 tex id
	static const unsigned int maxQuadCount = 10;
	static const unsigned int maxVertexCount = maxQuadCount * 3;
	static const unsigned int maxIndexCount = maxQuadCount * 6;
	static const unsigned int maxTextureSlots = 32; // Note for CoolFede97: Remember to change this according to the machine you are using!
	static const unsigned int mat4Size = sizeof(glm::mat4);
	static const unsigned int maxMat4Size = maxVertexCount * mat4Size;
	static const unsigned int maxTextureCountPerType = 3000;
}
