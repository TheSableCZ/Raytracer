//
// Created by Jan Sobol on 28/04/2021.
//

#ifndef RAYTRACER_OCTREEADS_H
#define RAYTRACER_OCTREEADS_H

#include "../AccelerationDS.h"
#include <array>

constexpr unsigned LEVEL_MAX = 4;
constexpr unsigned LIMIT_MAX = 10;

class OctreeNode {
public:
    OctreeNode(const glm::vec3 &center, const glm::vec3 &halfSize, unsigned level);
    OctreeNode(const AABB &bb, unsigned level);

    void insert(const std::shared_ptr<SceneObject> &object);
    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection);

    AABB bb;
    glm::vec3 center, halfSize;
    unsigned level;
    bool limitReached = false;

    std::array<std::shared_ptr<OctreeNode>, 8> children;
    std::array<glm::vec3, 8> childCenter;

    std::vector<std::shared_ptr<SceneObject>> objList;

private:
    void setCenters();
    void insertToChild(const std::shared_ptr<SceneObject> &object);
};

class OctreeADS : public AccelerationDS {
public:
    void clear() override
    {
        root.reset();
    }

    void insert(const std::shared_ptr<SceneObject> &object) override // TODO: mělo by se volat pouze přes buildOver
    {
        root->insert(object);
    }

    void insert(const std::vector<std::shared_ptr<SceneObject>> &object) override; // TODO: mělo by se volat pouze přes buildOver

    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override
    {
        return root->intersect(ray, tMin, tMax, intersection);
    }

private:
    std::shared_ptr<OctreeNode> root;
};


#endif //RAYTRACER_OCTREEADS_H
