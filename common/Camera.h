//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H


#include <glm/glm.hpp>
#include "Ray.h"
#include "../AppSettings.h"

class Camera {
public:
    Camera() = default;
    Camera (
        glm::vec3 lookfrom,
        glm::vec3 lookat,
        glm::vec3 vup,
        float vfov, // vertical field-of-view in degrees
        float aspectRatio,
        float aperture,
        float focusDist
    );

    void init (
            glm::vec3 lookfrom = AppSettings::lookfrom,
            glm::vec3 lookat = AppSettings::lookat,
            glm::vec3 vup = AppSettings::vup,
            float vfov = AppSettings::vfov, // vertical field-of-view in degrees
            float aspectRatio = static_cast<float>(AppSettings::imgWidth) / static_cast<float>(AppSettings::imgHeight),
            float aperture = AppSettings::aperture,
            float focusDist = AppSettings::distToFocus
    );

    Ray getRay(float s, float t) const;

private:
    glm::vec3 origin;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 lowerLeftCorner;
    glm::vec3 cameraDirection, cameraRight, cameraUp;
    float lensRadius;
};


#endif //RAYTRACER_CAMERA_H
