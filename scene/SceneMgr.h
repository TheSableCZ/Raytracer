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

class SceneMgr : public SceneObject {
public:
    SceneMgr() : SceneObject(std::make_unique<LinearADS>()) {}
    SceneMgr(std::unique_ptr<AccelerationDS> ads) : SceneObject(std::move(ads)) {}

    bool intersect(const Ray &ray, Intersection &intersection);
    Camera& camera() { return _camera; }
    std::shared_ptr<SceneObjsPdf> getLightPdf() const;
    inline unsigned lightSourcesCount() const { return lightSources.size(); }
    void prepare() override;
    void setLightAccelerationDS(std::unique_ptr<AccelerationDS> ds) { lightAccelerationDS = std::move(ds); }

private:
    Camera _camera;
    std::vector<std::shared_ptr<SceneObject>> lightSources;
    std::shared_ptr<SceneObjsPdf> lightPdf;
    std::unique_ptr<AccelerationDS> lightAccelerationDS;
};


#endif //RAYTRACER_SCENEMGR_H
