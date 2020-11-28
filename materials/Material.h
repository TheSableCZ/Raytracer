//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <glm/vec3.hpp>
#include "../common/Ray.h"
#include "../common/Intersection.h"

class Material {
public:
    virtual glm::vec3 emitted(const Ray &ray) { return glm::vec3(0); }
    virtual bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray& scatteredRay) const = 0;
};

class SimpleMat : public Material {
public:
    SimpleMat(glm::vec3 color) : color(color) {}
    glm::vec3 emitted(const Ray &ray) override { return color; }
    bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const override {
        return false;
    }

    glm::vec3 color;
};

#endif //RAYTRACER_MATERIAL_H
