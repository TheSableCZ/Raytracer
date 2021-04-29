//
// Created by Jan Sobol on 25/11/2020.
//

#include "SceneObject.h"
#include <iostream>
#include <algorithm>

SceneObject::SceneObject(std::unique_ptr<AccelerationDS> accelerationDS) : accelerationDS(std::move(accelerationDS)) {

}

SceneObject::SceneObject(const std::vector<std::shared_ptr<SceneObject>> &children, std::unique_ptr<AccelerationDS> accelerationDS)
    : children(children), accelerationDS(std::move(accelerationDS))
{
}

void SceneObject::prepare() {
    if (accelerationDS != nullptr) {
        std::vector<std::shared_ptr<SceneObject>> res;
        for (const auto &child : children) {
            auto lights = child->getLeafs();
            res.insert(res.end(), lights.begin(), lights.end());
        }
        accelerationDS->buildOver(res);
    }
    for (const auto &child : children) {
        child->prepare();
    }
}

void SceneObject::addChild(const std::shared_ptr<SceneObject>& sceneObj) {
    children.emplace_back(sceneObj);
    if (mat != nullptr) {
        children.back()->setMaterial(mat);
    }
}

void SceneObject::addChildren(const std::vector<std::shared_ptr<SceneObject>>& sceneObjs) {
    for (const auto& child : sceneObjs) {
        addChild(child);
    }
}

AABB SceneObject::getAABB() const {
    AABB res = {};
    for (const auto& child : children) {
        res = res.combine(child->getAABB());
    }
    return res;
}

std::vector<std::shared_ptr<SceneObject>> SceneObject::getLightSources() {
    if (isLightSource()) {
        return { shared_from_this() };
    } else {
        std::vector<std::shared_ptr<SceneObject>> res;
        for (const auto &child : children) {
            auto lights = child->getLightSources();
            res.insert(res.end(), lights.begin(), lights.end());
        }
        return res;
    }
}

std::vector<std::shared_ptr<SceneObject>> SceneObject::getLeafs() {
    if (isLeafNode() || accelerationDS != nullptr) {
        return { shared_from_this() };
    } else {
        std::vector<std::shared_ptr<SceneObject>> res;
        for (const auto &child : children) {
            auto lights = child->getLeafs();
            res.insert(res.end(), lights.begin(), lights.end());
        }
        return res;
    }
}

void SceneObject::transform(const glm::mat4 &matrix) {
    for (const auto &child : children) {
        child->transform(matrix);
    }
}

void SceneObject::setMaterial(const std::shared_ptr<Material> &mat) {
    this->mat = mat;
    for (const auto &child : children) {
        child->setMaterial(mat);
    }
}

unsigned long SceneObject::countPrimitives() const {
    if (isLeafNode()) {
        return 1;
    }
    unsigned long resCount = 0;
    for (const auto &child : children) {
        resCount += child->countPrimitives();
    }
    return resCount;
}
