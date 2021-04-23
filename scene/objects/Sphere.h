//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "../SceneObject.h"

class Sphere : public SceneObject {
public:
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat) : SceneObject(), center(center), radius(radius), mat(std::move(mat)) {}
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;
    virtual AABB getAABB() const override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
};


#endif //RAYTRACER_SPHERE_H
