//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "Material.h"
#include "../common/Random.h"

class Metal : public Material {
public:
    Metal(glm::vec3 albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const override;

    glm::vec3 albedo;
    float fuzz;
};

bool Metal::scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const {
    glm::vec3 reflected = reflect(normalize(inRay.direction), intersection.normal);
    scatteredRay = Ray(intersection.point, reflected + fuzz * randomInUnitSphere());
    attenuation = albedo;
    return (dot(scatteredRay.direction, intersection.normal) > 0);
}

#endif //RAYTRACER_METAL_H
