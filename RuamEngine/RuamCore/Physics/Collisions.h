#pragma once
#include "RuamEngine.h"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "RuamTime.h"
#include <climits>

struct AABB {
    float xMin = INT_MAX;
    float xMax = INT_MIN;
    float yMin = INT_MAX;
    float yMax = INT_MIN;
    float zMin = INT_MAX;
    float zMax = INT_MIN;
};