//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEMGR_H
#define RAYTRACER_SCENEMGR_H


#include <vector>
#include "SceneObject.h"
#include "../common/Camera.h"

class SceneObjsPdf;

class SceneMgr {
public:
    SceneMgr() = default;
    void addSceneObject(const std::shared_ptr<SceneObject>& sceneObj);
    std::vector<std::shared_ptr<SceneObject>>::const_iterator begin() const { return sceneObjs.cbegin(); }
    std::vector<std::shared_ptr<SceneObject>>::const_iterator end() const { return sceneObjs.cend(); }
    bool intersect(const Ray &ray, Intersection &intersection);
    Camera& camera() { return _camera; }
    std::shared_ptr<SceneObjsPdf> getLightPdf(const glm::vec3 &origin) const;
    unsigned lightSourcesCount() const { return lightSources.size(); }

private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjs;
    Camera _camera;
    std::vector<std::shared_ptr<SceneObject>> lightSources;
};


#endif //RAYTRACER_SCENEMGR_H
