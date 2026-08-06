#include "AABB.h"

namespace RuamEngine
{
    AABB TransformAABB(const AABB& aabb, const glm::mat4& transform)
    {
        glm::vec3 translation = glm::vec3(transform[3]);
        AABB transformedAABB = {{translation}, { translation}};

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                float e = transform[j][i] * aabb.min[j];
                float f = transform[j][i] * aabb.max[j];

                if (e < f)
                {
                    transformedAABB.min[i] += e;
                    transformedAABB.max[i] += f;
                }
                else
                {
                    transformedAABB.min[i] += f;
                    transformedAABB.max[i] += e;
                }
            }
        }
        return transformedAABB;
    }
}
