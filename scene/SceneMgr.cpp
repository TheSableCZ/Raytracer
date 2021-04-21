//
// Created by Jan Sobol on 25/11/2020.
//

#include "SceneMgr.h"
#include "../AppSettings.h"
#include "../common/Pdf.h"

std::shared_ptr<SceneObjsPdf> SceneMgr::getLightPdf(const glm::vec3 &origin) const {
    return std::make_shared<SceneObjsPdf>(lightSources, origin);
}

bool SceneMgr::intersect(const Ray &ray, Intersection &intersection) {
    return SceneObject::intersect(ray, AppSettings::tMin, AppSettings::tMax, intersection);
}

void SceneMgr::prepare() {
    SceneObject::prepare();
    lightSources = getLightSources(transform);
}
