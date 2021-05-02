//
// Created by Jan Sobol on 28/04/2021.
//

#include "OctreeADS.h"
#include "../SceneObject.h"
#include "../../common/MathUtils.h"
#include "../../AppSettings.h"
#include <iostream>

void OctreeADS::insert(const std::vector<std::shared_ptr<SceneObject>> &object) {
    AABB topLevel = {};
    for (const auto& obj : object) {
        topLevel = topLevel + obj->getAABB();
    }

    /*auto xmin = topLevel.minPos.x, xmax = topLevel.maxPos.x, ymin = topLevel.minPos.y, ymax = topLevel.maxPos.y,
    zmin = topLevel.minPos.z, zmax = topLevel.maxPos.z;

    constexpr float del = 1.f / 2.f;
    glm::vec3 center((xmin + xmax) * del, (ymin + ymax) * del, (zmin + zmax) * del);
    glm::vec3 halfSize((xmax - xmin) * del + eps, (ymax - ymin) * del + eps, (zmax - zmin) * del + eps);

    root = std::make_shared<OctreeNode>(center, halfSize, 0);*/

    root = std::make_shared<OctreeNode>(topLevel, 0);

    for(const auto& obj : object) {
        insert(obj);
    }
}

OctreeNode::OctreeNode(const glm::vec3 &center, const glm::vec3 &halfSize, unsigned int level)
    : bb(center - halfSize, center + halfSize), level(level), center(center), halfSize(halfSize)
{
    setCenters();
}

OctreeNode::OctreeNode(const AABB &bb, unsigned int level)
    : bb(bb), level(level)
{
    auto xmin = bb.minPos.x, xmax = bb.maxPos.x, ymin = bb.minPos.y, ymax = bb.maxPos.y,
            zmin = bb.minPos.z, zmax = bb.maxPos.z;

    constexpr float del = 0.5f;
    center = glm::vec3 ((xmin + xmax) * del, (ymin + ymax) * del, (zmin + zmax) * del);
    halfSize = glm::vec3 ((xmax - xmin) * del + eps, (ymax - ymin) * del + eps, (zmax - zmin) * del + eps);

    setCenters();
}

void OctreeNode::setCenters() {
    auto x = center.x, y = center.y, z = center.z;
    auto sx = halfSize.x, sy = halfSize.y, sz = halfSize.z;
    childCenter[0] = glm::vec3(x - sx / 2.0, y + sy / 2.0, z - sz / 2.0);
    childCenter[1] = glm::vec3(x + sx / 2.0, y + sy / 2.0, z - sz / 2.0);
    childCenter[2] = glm::vec3(x - sx / 2.0, y - sy / 2.0, z - sz / 2.0);
    childCenter[3] = glm::vec3(x + sx / 2.0, y - sy / 2.0, z - sz / 2.0);
    childCenter[4] = glm::vec3(x - sx / 2.0, y + sy / 2.0, z + sz / 2.0);
    childCenter[5] = glm::vec3(x + sx / 2.0, y + sy / 2.0, z + sz / 2.0);
    childCenter[6] = glm::vec3(x - sx / 2.0, y - sy / 2.0, z + sz / 2.0);
    childCenter[7] = glm::vec3(x + sx / 2.0, y - sy / 2.0, z + sz / 2.0);
}

void OctreeNode::insert(const std::shared_ptr<SceneObject> &object) {
    if (!limitReached) {
        objList.emplace_back(object);

        if (level < AppSettings::treeLevelMax && objList.size() >= AppSettings::treeLeafLimit) {
            limitReached = true;
            for (const auto &obj : objList) {
                insertToChild(obj);
            }
            objList.clear();
        }

    } else {
        insertToChild(object);
    }
}

void OctreeNode::insertToChild(const std::shared_ptr<SceneObject> &object) {
    const auto hhSize = halfSize * 0.5f;
    auto objBB = object->getAABB();

    for (int i = 0; i < 8; i++) {
        auto nc = childCenter[i];
        AABB nodeBB(nc - hhSize, nc + hhSize);
        if (nodeBB.bbInside(objBB)) { // TODO: zrychlení? nekolidovat AABBs, ale konkrétní objekt (polygon)
            if (!children[i]) {
                children[i] = std::make_shared<OctreeNode>(nodeBB, level + 1);
            }
            children[i]->insert(object);
        }
    }
}

bool OctreeNode::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    if (bb.isIntersecting(ray, tMin, tMax)) {

        if (!limitReached) {
            // linear, TODO? without AABB
            Intersection closestIntersection;
            auto intersected = false;
            auto closestSoFar = tMax;
            for (const auto& object : objList) {
                if (object->intersect(ray, tMin, closestSoFar, closestIntersection)) {
                    intersected = true;
                    closestSoFar = closestIntersection.t;
                    intersection = closestIntersection;
                }
            }
            return intersected;

        } else {
            Intersection closestIntersection;
            auto intersected = false;
            auto closestSoFar = tMax;

            for (const auto &node : children) {
                if(node
                    && node->bb.isIntersecting(ray, tMin, closestSoFar)
                    && node->intersect(ray, tMin, closestSoFar, closestIntersection))
                {
                    intersected = true;
                    closestSoFar = closestIntersection.t;
                    intersection = closestIntersection;
                }
            }

            return intersected;
        }

    } else {
        return false;
    }
}

float OctreeNode::pdfValue(const glm::vec3 &origin, const glm::vec3 &v) {
    Ray ray(origin, v);
    if (bb.isIntersecting(ray, AppSettings::tMin, AppSettings::tMax)) {

        if (!limitReached) {
            float res = 0.f;
            for (const auto& object : objList) {
                res += object->pdfValue(origin, v);
            }
            return res;

        } else {
            float res = 0.f;
            for (const auto &node : children) {
                if (node) {
                    res += node->pdfValue(origin, v);
                }
            }

            return res;
        }

    } else {
        return 0.f;
    }
}

void OctreeStats::createStats(const std::shared_ptr<OctreeNode> &root) {
    if (!root->limitReached) { // leaf
        histLeafObjCount[root->objList.size()]++;
        objCount += root->objList.size();
        histLevel[root->level]++;
        if (root->objList.size() > AppSettings::treeLeafLimit) {
            reachLimitAndMaxLevelCount++;
        }

        if (root->objList.size() > AppSettings::treeLeafLimit && root->level < AppSettings::treeLevelMax) {
            somethingWrong++;
        }
    } else {
        unsigned cnt = 0;
        for (const auto &child : root->children) {
            if (child) {
                createStats(child);
                cnt++;
            }
        }
        histNodeChildCount[cnt]++;
    }
    nodeCount++;
}

void OctreeStats::printStats() const {
    std::cout << "---------------------------------------------------\n";
    std::cout << "Octree Stats: \n";
    std::cout << "---------------------------------------------------\n";
    std::cout << "No Objects:  " << objCount << " in " << nodeCount << " octree nodes.\n";
    std::cout << "Reach limit but level is max: " << reachLimitAndMaxLevelCount << std::endl;
    std::cout << "Something wrong: " << somethingWrong << std::endl;

    printHist("Tree level (histogram)", histLevel);
    printHist("Objects in leaves (histogram)", histLeafObjCount);
    printHist("Number of children in nodes (histogram)", histNodeChildCount);
}

void OctreeStats::printHist(std::string title, const std::map<int, int> &hist) {
    std::cout << "---------------------------------------------------\n";
    std::cout << title << std::endl;
    std::cout << "---------------------------------------------------\n";
    for (const auto &pair : hist) {
        std::cout << "  " << pair.first << "\t:\t" << pair.second << std::endl;
    }
    std::cout << "---------------------------------------------------\n";
}
