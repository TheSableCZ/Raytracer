
#include "../SceneObject.h"
#include "AABBBVH.h"
#include <glm/gtx/component_wise.hpp>
#include "../../AppSettings.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// AABBBVH_node implementation
/////////////////////////////////////////////////////////////////////////////////

AABBBVH_node::AABBBVH_node(NodeArray children)
    : children(move(children))
{
    // build aabb from
    if (!this->children.empty()) {
        bb = this->children.front()->getBB();
        auto it = this->children.begin();
        ++it;
        for (; it != this->children.end(); ++it) {
            bb = bb + (*it)->getBB();
        }
    }
}

AABBBVH_node::AABBBVH_node(const std::shared_ptr<SceneObject> &obj)
    : obj(obj), bb(obj->getAABB())
{
}

int AABBBVH_node::buildTree(int level) {
    int depth = level;
    if (children.size() > AppSettings::treeLeafLimit
        && (
            AppSettings::AABBBVH_max_treeDepth < 0 ||
            level < AppSettings::AABBBVH_max_treeDepth
        )
    ) {
        split();
        for (const auto &child : children) {
            depth = std::max(child->buildTree(level + 1), depth);
        }
    } else if (children.size() == 1) {
        obj = children[0]->getObj();
        children.clear();
    }
    return depth;
}

void AABBBVH_node::split() {
    // object median split

    // get biggest dimension to split
    auto dimensions = bb.maxPos - bb.minPos;
    auto maxVal = glm::compMax(dimensions);

    // for sake of DRYness, I couldn't figure out better way ...
    #define sortByDimension(DIM)                        \
    sort(children.begin(), children.end(),              \
        []( const shared_ptr<AABBBVH_node> &a,          \
            const shared_ptr<AABBBVH_node> &b           \
        ) -> bool {                                     \
            return a->bb.maxPos.DIM > b->bb.maxPos.DIM; \
        }                                               \
    )

    if (dimensions.x == maxVal) {
        sortByDimension(x);
    } else if (dimensions.y == maxVal) {
        sortByDimension(y);
    } else {
        sortByDimension(z);
    }

    // https://stackoverflow.com/questions/9811235/best-way-to-split-a-vector-into-two-smaller-arrays
    auto median = children.size() / 2;
    NodeArray leftData(children.begin(), children.begin() + median);
    NodeArray right(children.begin() + median, children.end());

    children = {
        make_shared<AABBBVH_node>(move(leftData)),
        make_shared<AABBBVH_node>(move(right))
    };
}

NodeArray AABBBVH_node::getLeaves() const {
    NodeArray res = {};
    for (const auto &child : children) {
        if (child->isLeaf()) {
            res.emplace_back(child);
        } else {
            auto leaves = child->getLeaves();
            res.insert(res.end(), leaves.begin(), leaves.end());
        }
    }
    return res; // hopefully return -optimized
}

bool AABBBVH_node::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    if (!bb.isIntersecting(ray, tMin, tMax)) {
        return false;
    }

    if (isLeaf()) {
        return obj->intersect(ray, tMin, tMax, intersection);
    }

    // check children
    Intersection closestIntersection;
    auto intersected = false;
    auto closestSoFar = tMax;
    for (auto& child : children) {
        if (child->intersect(ray, tMin, closestSoFar, closestIntersection)) {
            intersected = true;
            closestSoFar = closestIntersection.t;
            intersection = closestIntersection;
        }
    }
    return intersected;
}

float AABBBVH_node::pdfValue(const glm::vec3 &origin, const glm::vec3 &v) {
    Ray ray(origin, v);
    if (bb.isIntersecting(ray, AppSettings::tMin, AppSettings::tMax)) {
        if (isLeaf()) {
            return obj->pdfValue(origin, v);
        } else {
            float res = 0.f;
            for (const auto &child : children) {
                if (child) {
                    res += child->pdfValue(origin, v);
                }
            }
            return res;
        }
    } else {
        return 0.f;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// AABBBVH implementation
/////////////////////////////////////////////////////////////////////////////////

void AABBBVH::insert(const std::vector<std::shared_ptr<SceneObject>> &objects) {
    NodeArray nodes = rootNode.getLeaves();
    std::transform(objects.begin(), objects.end(), std::back_inserter(nodes),
        [](const std::shared_ptr<SceneObject> &c) -> shared_ptr<AABBBVH_node> {
            return make_shared<AABBBVH_node>(c);
        }
    );
    rootNode = AABBBVH_node(move(nodes));
    treeDepth = rootNode.buildTree();
};

bool AABBBVH::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    return rootNode.intersect(ray, tMin, tMax, intersection);
}
