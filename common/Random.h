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

#endif //RAYTRACER_RANDOM_H
