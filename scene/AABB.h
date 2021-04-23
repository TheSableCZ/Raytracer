//
// Created by Petr Fusek on 20/04/2021.
//
#pragma once


#include <glm/glm.hpp>

class Ray;

// Quick workaround the fact that BB has to have at least some volume.
// this treshold value defines minimal width of AABB in one dimension and
// it should be set to minimal possible value. Current value is way too large, but works.
#define MINIMAL_WIDTH 0.01f
#define SET_MIN_WIDTH(coord) \
    if (std::abs(minPos.coord - maxPos.coord) < MINIMAL_WIDTH) { \
        minPos.coord -= MINIMAL_WIDTH * 0.5f; \
        maxPos.coord += MINIMAL_WIDTH * 0.5f; \
    }

/**
 * @brief An axis- aligned bounding box data
 */
struct AABB {
    AABB() = default;
    AABB(glm::vec3 min, glm::vec3 max) : minPos(min), maxPos(max) {
        SET_MIN_WIDTH(x);
        SET_MIN_WIDTH(y);
        SET_MIN_WIDTH(z);
    }

    glm::vec3 minPos = glm::vec3(FLT_MAX);
    glm::vec3 maxPos = glm::vec3(-FLT_MAX);

    inline AABB combine(const AABB& val) const {
        return AABB(glm::min(minPos, val.minPos), glm::max(maxPos, val.maxPos));
    }

    bool isIntersecting(const Ray &ray, float tMin, float tMax);
};
