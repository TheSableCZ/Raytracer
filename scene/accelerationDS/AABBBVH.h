#pragma once

#include "../AccelerationDS.h"
#include <algorithm>

struct AABBBVH_node;

using NodeArray = std::vector<std::shared_ptr<AABBBVH_node>>;

struct AABBBVH_node {

    AABBBVH_node(NodeArray children = {});
    AABBBVH_node(const std::shared_ptr<SceneObject> &obj);

    int buildTree(int level = 0);
    NodeArray getLeaves() const;
    float pdfValue(const glm::vec3 &origin, const glm::vec3 &v);

    // to make it fairly immutable
    inline AABB getBB() const { return bb; }
    inline const NodeArray &getChildren() const { return children; }
    inline const std::shared_ptr<SceneObject> &getObj() const { return obj; }
    inline bool isLeaf() { return obj != nullptr; }
    inline void addChild(std::shared_ptr<AABBBVH_node> child) { children.emplace_back(child); bb = bb + child->getBB(); }

    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection);
private:
    AABB bb;
    NodeArray children;
    std::shared_ptr<SceneObject> obj = nullptr;

private:
    void split();
};

class AABBBVH : public AccelerationDS {
public:

    AABBBVH(int leafCapacity = 2) {}

    virtual void clear() override {
        rootNode = AABBBVH_node(NodeArray{});
    };

    virtual void insert(const std::shared_ptr<SceneObject> &object) override {
        rootNode.addChild(std::make_shared<AABBBVH_node>(object));
    };

    virtual void insert(const std::vector<std::shared_ptr<SceneObject>> &objects) override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

    float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) override
    {
        return rootNode.pdfValue(origin, v);
    }

    int getDepth() const { return treeDepth; }

protected:
    int treeDepth = 0;
    AABBBVH_node rootNode;
};
