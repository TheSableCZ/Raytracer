//
// Created by Jan Sobol on 25/11/2020.
//

#include "SceneMgr.h"
#include "../AppSettings.h"
#include "../common/Pdf.h"

void SceneMgr::addSceneObject(const std::shared_ptr<SceneObject>& sceneObj) {
    sceneObjs.emplace_back(sceneObj);
    if (sceneObj->isLightSource()) {
        lightSources.emplace_back(sceneObj);
    }
}

bool SceneMgr::intersect(const Ray &ray, Intersection &intersection) {
    return accelerationDS->intersect(ray, AppSettings::tMin, AppSettings::tMax, intersection);
}

std::shared_ptr<SceneObjsPdf> SceneMgr::getLightPdf(const glm::vec3 &origin) const {
    return std::make_shared<SceneObjsPdf>(lightSources, origin);
}
