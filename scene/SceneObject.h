//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEOBJECT_H
#define RAYTRACER_SCENEOBJECT_H

#include "../common/Ray.h"
#include "../common/Intersection.h"
#include "AccelerationDS.h"

class SceneObject {
public:
    SceneObject() = default;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) = 0;
    virtual AABBValue getAABBValue() const { return {}; };

    virtual float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) { return 0.0; }
    virtual glm::vec3 randomDirection(const glm::vec3 &origin) const { return glm::vec3(0); }

    virtual bool isLightSource() const { return false; }
};

#endif //RAYTRACER_SCENEOBJECT_H
