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

glm::vec3 Raytracer::trace(const Ray &ray, int depth) {
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

    if (intersection.materialPtr->scatterByColors() && ray.colorComponent == -1) {
        auto tmpColor = glm::vec3 (0.f);
        for (int i = 0; i < 3; i++) {
            Ray rayCopy = ray;
            rayCopy.colorComponent = i;
            glm::vec3 attenuation;
            Ray scatteredRay;
            if (intersection.materialPtr->scatter(rayCopy, intersection, attenuation, scatteredRay)) {
                glm::vec3 result = attenuation * trace(scatteredRay, depth - 1);
                tmpColor += result;
            }
        }
        return tmpColor;
    } else {
        glm::vec3 attenuation;
        Ray scatteredRay;
        if (intersection.materialPtr->scatter(ray, intersection, attenuation, scatteredRay))
            return attenuation * trace(scatteredRay, depth - 1);
    }

    // zatím jenom jednoduché vrácení emitované barvy
    return intersection.materialPtr->emitted(ray);
    //return intersection.normal;
}

void Raytracer::render(ColorBuffer &colorBuffer, int width, int height) {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            glm::vec3 pixelColor(0, 0, 0);
            for (int s = 0; s < AppSettings::samplesPerPixel; ++s) {
                float u, v;
                u = (i + randomFloat()) / (width - 1);
                v = (j + randomFloat()) / (height - 1);
                Ray r = scene().camera().getRay(u, v);
                pixelColor += trace(r, AppSettings::maxDepth);
            }
            colorBuffer.p[(j * width) + i] = pixelColorOperation(pixelColor, AppSettings::samplesPerPixel);
        }
    }
}

void Raytracer::renderStage(ColorBuffer &colorBuffer, int width, int height) {
    for (int j = 0; j < height; ++j) {
        #pragma omp parallel for
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
            auto resultColor = clampColorWithRatio(trace(r, AppSettings::maxDepth));
            colorBuffer.p[(j * width) + i] += resultColor;
        }
    }
}

glm::vec3 Raytracer::pixelColorOperation(glm::vec3 pixelColor, int samplesPerPixel) {
    auto scale = 1.f / static_cast<float>(samplesPerPixel);
    return glm::vec3(sqrt(pixelColor.r * scale), sqrt(pixelColor.g * scale), sqrt(pixelColor.b * scale));
}

void Raytracer::initCameraWithAppSettings() {
    scene().camera().init(
            AppSettings::lookfrom,
            AppSettings::lookat,
            AppSettings::vup,
            AppSettings::vfov,
            static_cast<float>(AppSettings::imgWidth) / static_cast<float>(AppSettings::imgHeight),
            AppSettings::aperture,
            AppSettings::distToFocus
    );
}
