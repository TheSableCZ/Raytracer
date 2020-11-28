//
// Created by Jan Sobol on 25/11/2020.
//

#include "Camera.h"
#include "Random.h"

Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspectRatio, float aperture,
               float focusDist) {
    init(lookfrom, lookat, vup, vfov, aspectRatio, aperture, focusDist);
}

void Camera::init(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspectRatio, float aperture, float focusDist) {
    auto theta = glm::radians(vfov);
    auto h = tan(theta/2);
    float viewportHeight = 2.f * h;
    float viewportWidth = aspectRatio * viewportHeight;

    cameraDirection = glm::normalize(lookfrom - lookat);
    cameraRight = glm::normalize(cross(vup, cameraDirection));
    cameraUp = cross(cameraDirection, cameraRight);

    origin = lookfrom;
    horizontal = focusDist * viewportWidth * cameraRight;
    vertical = focusDist * viewportHeight * cameraUp;
    lowerLeftCorner = origin - horizontal / 2.f - vertical / 2.f - focusDist * cameraDirection;

    lensRadius = aperture / 2;
}

Ray Camera::getRay(float s, float t) const {
    glm::vec3 rd = lensRadius * randomInUnitDisk();
    glm::vec3 offset = cameraRight * rd.x + cameraUp * rd.y;
    return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
}
