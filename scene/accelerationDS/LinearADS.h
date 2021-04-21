//
// Created by Petr Fusek on 20/04/2021.
//

#pragma once

#include <memory>
#include <vector>
#include "../SceneObject.h"
#include "../AccelerationDS.h"

/**
 * @brief This class implements original unoptimized intersection seraching algorithm
 */
class LinearADS : public AccelerationDS {
public:

    virtual void clear() override {
        objects.clear();
    };

    virtual void insert(const std::shared_ptr<SceneObject> &object) override {
        objects.emplace_back(object);
    };

    virtual void insert(const std::vector<std::shared_ptr<SceneObject>> &objects) override {
        this->objects.reserve(this->objects.size() + objects.size());
        this->objects.insert(this->objects.end(), objects.begin(), objects.end());
    };

    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

protected:
    std::vector<std::shared_ptr<SceneObject>> objects = {};
};
