//
// Created by Jan Sobol on 09/12/2020.
//

#ifndef RAYTRACER_BSSRDF_H
#define RAYTRACER_BSSRDF_H

#include "Material.h"
#include "../common/Random.h"

constexpr auto magicScateringConstant = .0002f;

class BSSRDF : public Material {
public:
    BSSRDF(const glm::vec3 color, glm::vec3 sigma_t) : Material(), albedo(color), sigma_t(sigma_t) {}
    bool scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const override;
    bool scatterByColors() override { return true; }

    glm::vec3 albedo;
    glm::vec3 sigma_t;
};

bool
BSSRDF::scatter(const Ray &inRay, const Intersection &intersection, glm::vec3 &attenuation, Ray &scatteredRay) const {
    int colorComponent;
    if (inRay.colorComponent == -1) {
        auto rnd = randomFloat();
        if (rnd < 0.33333f) {
            colorComponent = 0;
        } else if (rnd >= 0.33333f and rnd < 0.66666f) {
            colorComponent = 1;
        } else {
            colorComponent = 2;
        }
    } else {
        colorComponent = inRay.colorComponent;
    }
    float _sigma_t = sigma_t[colorComponent];
    auto _attenuation = glm::vec3 (0.f);
    _attenuation[colorComponent] = 1.f;

    if (intersection.frontFace) {
        scatteredRay = Ray(
                inRay.at(intersection.t + magicScateringConstant),
                -intersection.normal + randomUnitVector()
                );
        attenuation = albedo;
        scatteredRay.colorComponent = colorComponent;
        return true;
    } else {
        float so = -log(randomFloat()) / _sigma_t;
        float si = glm::length(intersection.point - inRay.origin);
        if (si <= so) //turns into exitant, go out of the objects
        {
            scatteredRay = Ray(
                    inRay.at(intersection.t + magicScateringConstant),
                    -intersection.normal + randomUnitVector()
                    );
            attenuation = _attenuation;
            scatteredRay.colorComponent = colorComponent;
            return true;
            //ray.origin = getPointOnRay(ray, intrT + .0002f);
            //ray.direction = glm::normalize(calculateRandomDirectionInHemisphere(-normal, rrr));
        }
        else //stays in the obj, pick new direction and scatter distance
        {
            scatteredRay = Ray(
                    inRay.at(so),
                    -(inRay.direction + randomUnitVector())
                    );
            attenuation = _attenuation;
            scatteredRay.colorComponent = colorComponent;
            return true;
            //ray.origin = getPointOnRay(ray, so);
            //ray.direction = -glm::normalize(calculateRandomDirectionInHemisphere(ray.direction, rrr));
        }
    }
}

#endif //RAYTRACER_BSSRDF_H
