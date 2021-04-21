//
// Created by Jan Sobol on 09/12/2020.
//

#ifndef RAYTRACER_PLANE_H
#define RAYTRACER_PLANE_H

#include <utility>

#include "../SceneObject.h"

class Plane : public SceneObject {
public:
    Plane(glm::vec3 normal, glm::vec3 point, std::shared_ptr<Material> mat)
        : SceneObject(), n(normalize(normal)), d(-(dot(n,point))), mat(std::move(mat)) {}
    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

private:
    glm::vec3 n;
    float d;
    std::shared_ptr<Material> mat;
};


#endif //RAYTRACER_PLANE_H
