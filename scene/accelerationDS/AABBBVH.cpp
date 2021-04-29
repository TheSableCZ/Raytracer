
#include "AABBBVH.h"
#include <glm/gtx/component_wise.hpp>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// AABBBVH_node implementation
/////////////////////////////////////////////////////////////////////////////////

AABBBVH_node::AABBBVH_node(NodeArray children)
    : children(move(children))
{
    // build aabb from
    if (!children.empty()) {
        bb = children.front()->getBB();
        auto it = children.begin();
        std::advance(it, 1);
        for (; it != children.end(); ++it) {
            bb.combine((*it)->getBB());
        }
    }
}

AABBBVH_node::AABBBVH_node(const std::shared_ptr<SceneObject> &obj)
    : obj(obj), bb(obj->getAABB())
{
}


int AABBBVH_node::buildTree(int leafCapacity, int level) {
    int depth = 0;
    if (children.size() > leafCapacity) {
        split();
        for (const auto &child : children) {
            depth = max(child->buildTree(leafCapacity, level + 1), depth);
        }
    }
    return level + depth;
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
    rootNode.buildTree(leafCapacity);
};

void AABBBVH::buildOver(const std::vector<std::shared_ptr<SceneObject>> &objects) {

    NodeArray nodes;

    // create nodes from objects
    std::transform(objects.begin(), objects.end(), std::back_inserter(nodes),
        [](const std::shared_ptr<SceneObject> &c) -> shared_ptr<AABBBVH_node> {
            return make_shared<AABBBVH_node>(c);
        }
    );

    rootNode = AABBBVH_node(move(nodes));
}

bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    return false;
}
