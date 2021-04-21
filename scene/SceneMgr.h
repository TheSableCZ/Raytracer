//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEMGR_H
#define RAYTRACER_SCENEMGR_H


#include <vector>
#include "SceneObject.h"
#include "AccelerationDS.h"
#include "accelerationDS/LinearADS.h"
#include "../common/Camera.h"

class SceneObjsPdf;

class SceneMgr {
public:
    SceneMgr() : accelerationDS(std::make_unique<LinearADS>()) {}
    SceneMgr(std::unique_ptr<AccelerationDS> ads) : accelerationDS(std::move(ads)) {}

    void addSceneObject(const std::shared_ptr<SceneObject>& sceneObj);
    std::vector<std::shared_ptr<SceneObject>>::const_iterator begin() const { return sceneObjs.cbegin(); }
    std::vector<std::shared_ptr<SceneObject>>::const_iterator end() const { return sceneObjs.cend(); }
    bool intersect(const Ray &ray, Intersection &intersection);
    Camera& camera() { return _camera; }
    std::shared_ptr<SceneObjsPdf> getLightPdf(const glm::vec3 &origin) const;
    inline unsigned lightSourcesCount() const { return lightSources.size(); }
    inline void prepare() { accelerationDS->buildOver(sceneObjs); };

private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjs;
    Camera _camera;
    std::vector<std::shared_ptr<SceneObject>> lightSources;
    std::unique_ptr<AccelerationDS> accelerationDS;
};


#endif //RAYTRACER_SCENEMGR_H
