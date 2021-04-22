//
// Created by Petr Fusek on 20/04/2021.
//
#pragma once


#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../common/Ray.h"
#include "../common/Intersection.h"

/**
 * @brief An axis- aligned bounding box data
 */
struct AABBValue {
    glm::vec3 minPos = glm::vec3(0);
    glm::vec3 maxPos = glm::vec3(0);

    bool isIntersecting(const Ray &ray) const { return false; }
    AABBValue combine(const AABBValue& val) const {
        return AABBValue{ glm::min(minPos, val.minPos), glm::max(maxPos, val.maxPos) };
    }
};

/**
 * @brief Acceleration datastructure of spacial objects
 */
class AccelerationDS {
public:
    virtual void clear() = 0;
    virtual void insert(const std::shared_ptr<SceneObject> &object) = 0;
    virtual void insert(const std::vector<std::shared_ptr<SceneObject>> &object) = 0;
    virtual void buildOver(const std::vector<std::shared_ptr<SceneObject>> &objects) {
        clear();
        insert(objects);
    };

    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) = 0;
};
