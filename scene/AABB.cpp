//
// Created by Petr Fusek on 20/04/2021.
//

#include "AABB.h"
#include "../common/Ray.h"
#include <glm/gtx/component_wise.hpp>

/**
 * @see https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
 */
bool AABB::isIntersecting(const Ray &ray, float tMin, float tMax) {
    auto invD = 1.0f / ray.direction;

    auto tmpTMin = (minPos - ray.origin) * invD;
    auto tmpTMax = (maxPos - ray.origin) * invD;

    auto tSmaller = glm::min(tmpTMin, tmpTMax);
    auto tBigger = glm::max(tmpTMin, tmpTMax);

    tMin = glm::max(tMin, glm::compMax(tSmaller));
    tMax = glm::min(tMax, glm::compMin(tBigger));

    return tMin < tMax;
}
