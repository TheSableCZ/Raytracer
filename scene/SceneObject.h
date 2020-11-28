//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEOBJECT_H
#define RAYTRACER_SCENEOBJECT_H

#include "../common/Ray.h"
#include "../common/Intersection.h"
#include "../materials/Material.h"

class SceneObject {
public:
    SceneObject(std::shared_ptr<Material> &mat) : material(mat) {}
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) = 0;

    std::shared_ptr<Material> material;
};

#endif //RAYTRACER_SCENEOBJECT_H
