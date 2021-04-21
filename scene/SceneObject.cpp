//
// Created by Jan Sobol on 25/11/2020.
//

#include "SceneObject.h"
#include <iostream>

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
}

bool SceneObject::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    Ray actRay = ray;
    actRay.origin = glm::inverse(transform) * glm::vec4(actRay.origin, 1.0f);
    actRay.direction = glm::inverse(transform) * glm::vec4(actRay.direction, 0.0f);
    float scaleFactor = glm::length(ray.direction) / glm::length(actRay.direction);
    actRay.direction = glm::normalize(actRay.direction);

    if (msg != "") {
        std::cout << "crrr...";
    }

    if (accelerationDS != nullptr) {
        if (accelerationDS->intersect(actRay, tMin, tMax, intersection)) {
            intersection.point = transform * glm::vec4(intersection.point, 1.0f);
            intersection.normal = transform * glm::vec4(intersection.normal, 1.0f);
            intersection.t *= scaleFactor;
            return true;
        }
    }
    return false;
}

AABBValue SceneObject::getAABBValue() const {
    return {};
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

void SceneObject::makeLight() {
    if (isLeafNode()) {
        lightSource = true;
    } else {
        for (const auto &child : children) {
            child->makeLight();
        }
    }
}
