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
    bool scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo, int colorChannel) const override;
    float scatteringPdf(const Ray &inRay, const Intersection &intersection, const Ray &scatteredRay,
                        const ScatterInfo &scatterInfo) const override;

    glm::vec3 albedo;
};

bool Lambertian::scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo,
                         int colorChannel) const {
    if (!AppSettings::useMC) {
        //auto scatter_direction = intersection.normal + randomUnitVector();
        auto scatter_direction = calculateRandomDirectionInHemisphere(intersection.normal);

        // Catch degenerate scatter direction
        if (nearZero(scatter_direction))
            scatter_direction = intersection.normal;

        scatterInfo.scatteredRay = Ray(intersection.point, scatter_direction);
        scatterInfo.attenuation = albedo;
        return true;
    } else {
        scatterInfo.attenuation = albedo;
        scatterInfo.pdfPtr = std::make_shared<CosinePdf>(intersection.normal);
        scatterInfo.rayOrigin = intersection.point;
        scatterInfo.useMC = true;
        return true;
    }
}

float Lambertian::scatteringPdf(const Ray &inRay, const Intersection &intersection, const Ray &scatteredRay,
                                const ScatterInfo &scatterInfo) const {
    auto cosine = dot(normalize(scatteredRay.direction), intersection.normal);
    return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
}

#endif //RAYTRACER_LAMBERTIAN_H
