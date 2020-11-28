//
// Created by Jan Sobol on 27/11/2020.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "Material.h"
#include "../common/Random.h"
#include "../common/MathUtils.h"

class Lambertian : public Material {
public:
    explicit Lambertian(const glm::vec3 color) : albedo(color) {}
    bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const override;

    glm::vec3 albedo;
};

bool Lambertian::scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation,
                         Ray &scatteredRay) const {
    auto scatter_direction = intersection.normal + randomUnitVector();

    // Catch degenerate scatter direction
    if (nearZero(scatter_direction))
        scatter_direction = intersection.normal;

    scatteredRay = Ray(intersection.point, scatter_direction);
    attenuation = albedo;
    return true;
}

#endif //RAYTRACER_LAMBERTIAN_H
