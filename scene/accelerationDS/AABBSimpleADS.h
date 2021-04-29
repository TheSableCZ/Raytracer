//
// Created by Petr Fusek on 20/04/2021.
//

#pragma once

#include <memory>
#include <vector>
#include "../SceneObject.h"
#include "../AccelerationDS.h"
#include <algorithm>

struct AABBSimpleADS_node {
    AABBSimpleADS_node(const std::shared_ptr<SceneObject> &obj)
        : obj(obj), bb(obj->getAABB()) {}
    AABB bb;
    std::shared_ptr<SceneObject> obj;
};

/**
 * @brief This class implements original unoptimized intersection seraching algorithm
 */
class AABBSimpleADS : public AccelerationDS {
public:

    virtual void clear() override {
        nodes.clear();
    };

    virtual void insert(const std::shared_ptr<SceneObject> &object) override {
        nodes.emplace_back(AABBSimpleADS_node(object));
    };

    virtual void insert(const std::vector<std::shared_ptr<SceneObject>> &objects) override {
        std::transform(objects.begin(), objects.end(), std::back_inserter(nodes),
            [](const std::shared_ptr<SceneObject> &c) -> AABBSimpleADS_node { return AABBSimpleADS_node(c); }
        );
    };

    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

protected:
    std::vector<AABBSimpleADS_node> nodes = {};
};
