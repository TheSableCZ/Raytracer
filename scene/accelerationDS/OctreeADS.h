//
// Created by Jan Sobol on 28/04/2021.
//

#ifndef RAYTRACER_OCTREEADS_H
#define RAYTRACER_OCTREEADS_H

#include "../AccelerationDS.h"
#include <array>
#include <map>

//constexpr unsigned LEVEL_MAX = 8;
//constexpr unsigned LIMIT_MAX = 10;

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

class OctreeStats {
public:
    void createStats(const std::shared_ptr<OctreeNode> &root);
    void printStats() const;
    static void printHist(std::string title, const std::map<int, int> &hist);

    std::map<int, int> histLevel = {};
    std::map<int, int> histLeafObjCount= {};
    std::map<int, int> histNodeChildCount = {};
    unsigned objCount = 0;
    unsigned nodeCount = 0;
    unsigned reachLimitAndMaxLevelCount = 0;

    unsigned somethingWrong = 0;
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

    void stats() {
        OctreeStats stats;
        stats.createStats(root);
        stats.printStats();
    }

private:
    std::shared_ptr<OctreeNode> root;
};


#endif //RAYTRACER_OCTREEADS_H
