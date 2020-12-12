//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_RANDOM_H
#define RAYTRACER_RANDOM_H

#include <cstdlib>
#include <glm/gtx/norm.hpp>

inline float randomFloat() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline float randomFloat(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max-min)*randomFloat();
}

inline glm::vec3 randomVec3(float min, float max) {
    return glm::vec3 (randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

inline int randomInt(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(randomFloat(min, max+0.9999));
}

inline glm::vec3 randomInUnitSphere() {
    while (true) {
        auto p = randomVec3(-1, 1);
        if (length2(p) >= 1) continue;
        return p;
    }
}

inline glm::vec3 randomUnitVector() {
    return normalize(randomInUnitSphere());
}

inline glm::vec3 randomInUnitDisk() {
    while (true) {
        auto p = glm::vec3(randomFloat(-1,1), randomFloat(-1,1), 0);
        if (length2(p) >= 1) continue;
        return p;
    }
}

#define TWO_PI            6.2831853071795864769252867665590057683943f
#define SQRT_OF_ONE_THIRD 0.5773502691896257645091487805019574556476f

/**
 * Computes a cosine-weighted random direction in a hemisphere.
 * Used for diffuse lighting.
 *
 * Copied from https://github.com/zammiez/Project3-CUDA-Path-Tracer
 */
inline glm::vec3 calculateRandomDirectionInHemisphere(glm::vec3 normal) {

    float up = sqrt(randomFloat()); // cos(theta)
    float over = sqrt(1 - up * up); // sin(theta)
    float around = randomFloat() * TWO_PI;

    // Find a direction that is not the normal based off of whether or not the
    // normal's components are all equal to sqrt(1/3) or whether or not at
    // least one component is less than sqrt(1/3). Learned this trick from
    // Peter Kutz.

    glm::vec3 directionNotNormal;
    if (abs(normal.x) < SQRT_OF_ONE_THIRD) {
        directionNotNormal = glm::vec3(1, 0, 0);
    } else if (abs(normal.y) < SQRT_OF_ONE_THIRD) {
        directionNotNormal = glm::vec3(0, 1, 0);
    } else {
        directionNotNormal = glm::vec3(0, 0, 1);
    }

    // Use not-normal direction to generate two perpendicular directions
    glm::vec3 perpendicularDirection1 =
            glm::normalize(glm::cross(normal, directionNotNormal));
    glm::vec3 perpendicularDirection2 =
            glm::normalize(glm::cross(normal, perpendicularDirection1));

    return up * normal
           + cos(around) * over * perpendicularDirection1
           + sin(around) * over * perpendicularDirection2;
}

#endif //RAYTRACER_RANDOM_H
