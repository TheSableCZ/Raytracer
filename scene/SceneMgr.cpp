//
// Created by Jan Sobol on 25/11/2020.
//

#include "SceneMgr.h"
#include "../AppSettings.h"

void SceneMgr::addSceneObject(const std::shared_ptr<SceneObject>& sceneObj) {
    sceneObjs.emplace_back(sceneObj);
}

bool SceneMgr::intersect(const Ray &ray, Intersection &intersection) {
    Intersection tempRec;
    auto intersect = false;
    auto closestSoFar = AppSettings::tMax;

    for (const auto& object : sceneObjs) {
        if (object->intersect(ray, AppSettings::tMin, closestSoFar, tempRec)) {
            intersect = true;
            closestSoFar = tempRec.t;
            intersection = tempRec;
        }
    }

    return intersect;
}
