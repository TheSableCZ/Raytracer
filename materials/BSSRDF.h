//
// Created by Jan Sobol on 09/12/2020.
//

#ifndef RAYTRACER_BSSRDF_H
#define RAYTRACER_BSSRDF_H

#include "Material.h"
#include "../common/Random.h"

class BSSRDF : public Material {
public:
    BSSRDF(const glm::vec3 &color, const glm::vec3 &sigma_t) : Material(), albedo(color), sigma_t(sigma_t), singleSigma(false) {}
    BSSRDF(const glm::vec3 &color, float sigma_t) : Material(), albedo(color), sigma_t(glm::vec3(sigma_t)), singleSigma(true) {}
    bool scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo, int colorChannel) const override;
    bool scatterByColors() override { return !singleSigma; }
    float scatteringPdf(const Ray &inRay, const Intersection &intersection, const Ray &scatteredRay,
                        const ScatterInfo &scatterInfo) const override;

    glm::vec3 albedo;
    glm::vec3 sigma_t;
    bool singleSigma;
};


constexpr auto jumpInsideConstant = .0002f;

bool
BSSRDF::scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo, int colorChannel) const {
    auto &attenuation = scatterInfo.attenuation;
    auto &scatteredRay = scatterInfo.scatteredRay;
    auto &scatteredRayType = scatterInfo.scatteredRayType;

    float _sigma_t;
    glm::vec3 _attenuation(0.f);

    if (singleSigma) {
        _sigma_t = sigma_t.r;
        _attenuation = glm::vec3 (1.f);
    } else {
        _sigma_t = sigma_t[colorChannel];
        _attenuation[colorChannel] = 1.f;
    }

    float so;
    if (intersection.frontFace) {
        scatteredRayType = ScatteredRayType::BSSRDF_enteringMedium;
        attenuation = albedo;
    } else {
        so = -log(randomFloat()) / _sigma_t;
        float si = glm::length(intersection.point - inRay.origin);
        if (si <= so) //turns into exitant, go out of the objects
        {
            scatteredRayType = ScatteredRayType::BSSRDF_exitingMedium;
            attenuation = _attenuation;
        }
        else //stays in the obj, pick new direction and scatter distance
        {
            scatteredRayType = ScatteredRayType::BSSRDF_insideMedium;
            attenuation = _attenuation;
        }
    }

    if (!AppSettings::useMC) {
        if (scatteredRayType == ScatteredRayType::BSSRDF_insideMedium) {
            scatteredRay = Ray(
                    inRay.at(so),
                    -normalize(calculateRandomDirectionInHemisphere(
                            inRay.direction))
            );
        } else {
            scatteredRay = Ray(
                    inRay.at(intersection.t + jumpInsideConstant),
                    normalize(calculateRandomDirectionInHemisphere(
                            -intersection.normal))
            );
        }
    } else {
        scatterInfo.useMC = true;

        if (scatteredRayType == ScatteredRayType::BSSRDF_insideMedium) {
            scatterInfo.pdfPtr = std::make_shared<MinusCosinePdf>(inRay.direction);
            scatterInfo.rayOrigin = inRay.at(so);
        } else {
            scatterInfo.pdfPtr = std::make_shared<CosinePdf>(-intersection.normal);
            scatterInfo.rayOrigin = inRay.at(intersection.t + jumpInsideConstant);
        }
    }

    return true;
}

float BSSRDF::scatteringPdf(const Ray &inRay, const Intersection &intersection, const Ray &scatteredRay,
                            const ScatterInfo &scatterInfo) const {
    glm::vec3 hemisphereDir;
    glm::vec3 scatteredDir;
    if (scatterInfo.scatteredRayType == ScatteredRayType::BSSRDF_insideMedium) {
        hemisphereDir =  normalize(inRay.direction);
        scatteredDir =  -normalize(scatteredRay.direction);
    } else {
        hemisphereDir = -intersection.normal;
        scatteredDir =   normalize(scatteredRay.direction);
    }

    auto cosine = dot(scatteredDir, hemisphereDir);
    return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
}

#endif //RAYTRACER_BSSRDF_H
