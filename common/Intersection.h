//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include <glm/vec3.hpp>
#include <memory>

class SceneObject;

struct Intersection {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    bool frontFace = true;
    std::shared_ptr<SceneObject> objectPtr;

    void setFaceAndNormal(const glm::vec3 rayDirection, const glm::vec3 computedNormal) {
        frontFace = dot(rayDirection, computedNormal) < 0;
        normal = frontFace ? computedNormal :-computedNormal;
    }
};

#endif //RAYTRACER_INTERSECTION_H
