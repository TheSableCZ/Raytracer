//
// Created by Jan Sobol on 26/11/2020.
//

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "Material.h"
#include "../common/Random.h"

class Dielectric : public Material {
public:
    Dielectric(float indexOfRefraction) : ir(indexOfRefraction) {}
    bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const override;

private:
    float ir;

    static float reflectance(float cosine, float refIdx) {
        // Use Schlick's approximation for reflectance.
        float r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

bool Dielectric::scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation,
                         Ray &scatteredRay) const {
    attenuation = glm::vec3 (1.0, 1.0, 1.0);
    auto refraction_ratio = intersection.frontFace ? (1.f/ir) : ir;

    glm::vec3 unit_direction = normalize(inRay.direction);
    auto cos_theta = fmin(dot(-unit_direction, intersection.normal), 1.f);
    auto sin_theta = sqrt(1.f - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    glm::vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomFloat())
        direction = reflect(unit_direction, intersection.normal);
    else
        direction = refract(unit_direction, intersection.normal, refraction_ratio);

    scatteredRay = Ray(intersection.point, direction);
    return true;
}

#endif //RAYTRACER_DIELECTRIC_H
