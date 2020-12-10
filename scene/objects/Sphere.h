//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "../SceneObject.h"

class Sphere : public SceneObject, public std::enable_shared_from_this<Sphere> {
public:
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat) : SceneObject(), center(center), radius(radius), mat(std::move(mat)) {}
    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
};


#endif //RAYTRACER_SPHERE_H
