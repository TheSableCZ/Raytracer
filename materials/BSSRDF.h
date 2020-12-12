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
    bool scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo) const override;
    bool scatterByColors() override { return !singleSigma; }
    float scatteringPdf(const Ray &inRay, const Intersection &intersection, const Ray &scatteredRay,
                        const ScatterInfo &scatterInfo) const override;

    glm::vec3 albedo;
    glm::vec3 sigma_t;
    bool singleSigma;
};


constexpr auto magicScateringConstant = .0002f;

bool BSSRDF::scatter(const Ray &inRay, const Intersection &intersection, ScatterInfo &scatterInfo) const {
    auto &attenuation = scatterInfo.attenuation;
    auto &scatteredRay = scatterInfo.scatteredRay;
    auto &scatteredRayType = scatterInfo.scatteredRayType;

    float _sigma_t;
    glm::vec3 _attenuation(0.f);
    int colorComponent = inRay.colorComponent;

    if (singleSigma) {
        _sigma_t = sigma_t.r;
        _attenuation = glm::vec3 (1.f);
    } else {
        _sigma_t = sigma_t[colorComponent];
        _attenuation[colorComponent] = 1.f;
    }

    float so;
    if (intersection.frontFace) {
        //scatteredRay = Ray(
        //        inRay.at(intersection.t + magicScateringConstant),
        //        normalize(calculateRandomDirectionInHemisphere(-intersection.normal)) //-intersection.normal + randomUnitVector()
        //        );
        scatteredRayType = ScatteredRayType::BSSRDF_enteringMedium;
        attenuation = albedo;
        //scatteredRay.colorComponent = colorComponent;
        //return true;
    } else {
        so = -log(randomFloat()) / _sigma_t;
        float si = glm::length(intersection.point - inRay.origin);
        if (si <= so) //turns into exitant, go out of the objects
        {
            //scatteredRay = Ray(
            //        inRay.at(intersection.t + magicScateringConstant),
            //        normalize(calculateRandomDirectionInHemisphere(-intersection.normal)) //-intersection.normal + randomUnitVector()
            //        );
            scatteredRayType = ScatteredRayType::BSSRDF_exitingMedium;
            attenuation = _attenuation;
            //scatteredRay.colorComponent = colorComponent;
            //return true;
            //ray.origin = getPointOnRay(ray, intrT + .0002f);
            //ray.direction = glm::normalize(calculateRandomDirectionInHemisphere(-normal, rrr));
        }
        else //stays in the obj, pick new direction and scatter distance
        {
            //scatteredRay = Ray(
            //        inRay.at(so),
            //        -normalize(calculateRandomDirectionInHemisphere(inRay.direction)) //-(inRay.direction + randomUnitVector())
            //        );
            scatteredRayType = ScatteredRayType::BSSRDF_insideMedium;
            attenuation = _attenuation;
            //scatteredRay.colorComponent = colorComponent;
            //return true;
            //ray.origin = getPointOnRay(ray, so);
            //ray.direction = -glm::normalize(calculateRandomDirectionInHemisphere(ray.direction, rrr));
        }
    }

    //attenuation = albedo;

    if (!AppSettings::useMC) {
        if (scatteredRayType == ScatteredRayType::BSSRDF_insideMedium) {
            scatteredRay = Ray(
                    inRay.at(so),
                    -normalize(calculateRandomDirectionInHemisphere(
                            inRay.direction)) //-(inRay.direction + randomUnitVector())
            );
        } else {
            scatteredRay = Ray(
                    inRay.at(intersection.t + magicScateringConstant),
                    normalize(calculateRandomDirectionInHemisphere(
                            -intersection.normal)) //-intersection.normal + randomUnitVector()
            );
        }
        scatteredRay.colorComponent = colorComponent;
    } else {
        scatterInfo.useMC = true;
        scatterInfo.colorChannel = colorComponent;

        if (scatteredRayType == ScatteredRayType::BSSRDF_insideMedium) {
            scatterInfo.pdfPtr = std::make_shared<MinusCosinePdf>(inRay.direction);
            scatterInfo.rayOrigin = inRay.at(so);
        } else {
            scatterInfo.pdfPtr = std::make_shared<CosinePdf>(-intersection.normal);
            scatterInfo.rayOrigin = inRay.at(intersection.t + magicScateringConstant);
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
