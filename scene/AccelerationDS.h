//
// Created by Petr Fusek on 20/04/2021.
//
#pragma once


#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "AABB.h"
#include "../common/Ray.h"
#include "../common/Intersection.h"

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
