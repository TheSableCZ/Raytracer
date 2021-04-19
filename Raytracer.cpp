//
// Created by Jan Sobol on 25/11/2020.
//

#include "Raytracer.h"
#include "common/Random.h"
#include "AppSettings.h"
#include "common/MathUtils.h"
#include "materials/Material.h"

Raytracer::Raytracer() {
    initCameraWithAppSettings();
}

glm::vec3 Raytracer::trace(const Ray &ray, int depth, int colorChannel) {
    if (depth <= 0)
        return glm::vec3(0.f);

    Intersection intersection;
    if (!sceneMgr.intersect(ray, intersection))
        return AppSettings::backgroundColor;

#ifndef NDEBUG
    if (AppSettings::debug_showNormals)
        return intersection.normal;
    if (AppSettings::debug_showFrontSides)
        return intersection.frontFace ? glm::vec3 (1.f, 0.f, 0.f) : glm::vec3 (0.f, 0.f, 1.f);
#endif

    if (intersection.materialPtr->scatterByColors() && colorChannel == -1) {
        auto tmpColor = glm::vec3 (0.f);
        for (int i = 0; i < 3; i++) {
            Ray rayCopy = ray;
            ScatterInfo scatterInfo;
            if (intersection.materialPtr->scatter(rayCopy, intersection, scatterInfo, i)) {
                //glm::vec3 result = attenuation * trace(scatteredRay, depth - 1);
                tmpColor += calculateScatteredRay(rayCopy, intersection, scatterInfo, depth, i);
            }
        }
        return tmpColor;
    } else {
        ScatterInfo scatterInfo;
        if (intersection.materialPtr->scatter(ray, intersection, scatterInfo, colorChannel))
            //return attenuation * trace(scatteredRay, depth - 1);
            return calculateScatteredRay(ray, intersection, scatterInfo, depth, colorChannel);
    }

    return intersection.materialPtr->emitted(ray);
}

inline glm::vec3 Raytracer::calculateScatteredRay(
    const Ray &inRay,
    const Intersection &intersection,
    const ScatterInfo &scatterInfo,
    int depth,
    int colorChannel
) {
    if (scatterInfo.useMC) {
        std::shared_ptr<Pdf> p;
        if (AppSettings::lightsDirectSampling && scene().lightSourcesCount() > 0
                && scatterInfo.scatteredRayType != ScatteredRayType::BSSRDF_enteringMedium
                && scatterInfo.scatteredRayType != ScatteredRayType::BSSRDF_insideMedium
                )
        {
            p = std::make_shared<MixturePdf>(scatterInfo.pdfPtr, scene().getLightPdf(scatterInfo.rayOrigin));
        } else {
            p = scatterInfo.pdfPtr;
        }

#ifndef NDEBUG
        if (AppSettings::debug_sampleOnlyLights) {
            p = scene().getLightPdf(scatterInfo.rayOrigin);
        }
#endif

        Ray scattered = Ray(scatterInfo.rayOrigin, p->generate());
        auto pdfValue = p->value(scattered.direction);

        return scatterInfo.attenuation * intersection.materialPtr->scatteringPdf(inRay, intersection, scattered, scatterInfo)
               * trace(scattered, depth - 1, colorChannel)
               / pdfValue;
    } else {
        return scatterInfo.attenuation * trace(scatterInfo.scatteredRay, depth - 1, colorChannel);
    }
}

void Raytracer::renderStage(ColorBuffer &colorBuffer, int width, int height) {
    #pragma omp parallel for collapse(2) schedule(dynamic, 600)
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float u, v;
            if (AppSettings::antialiasing) {
                u = (static_cast<float>(i) + randomFloat()) / static_cast<float>(width - 1);
                v = (static_cast<float>(j) + randomFloat()) / static_cast<float>(height - 1);
            } else {
                u = static_cast<float>(i) / static_cast<float>(width - 1);
                v = static_cast<float>(j) / static_cast<float>(height - 1);
            }
            Ray r = scene().camera().getRay(u, v);
            auto resultColor = trace(r, AppSettings::maxDepth, -1);
            colorBuffer.p[(j * width) + i] += resultColor;
        }
    }
}

glm::vec3 Raytracer::pixelColorOperation(glm::vec3 pixelColor, int samplesPerPixel) {
    auto scale = 1.f / static_cast<float>(samplesPerPixel);
    pixelColor = deNan(pixelColor);
    return clampColorWithRatio(glm::vec3(sqrt(pixelColor.r * scale), sqrt(pixelColor.g * scale), sqrt(pixelColor.b * scale)));
}

void Raytracer::initCameraWithAppSettings() {
    scene().camera().init();
}
