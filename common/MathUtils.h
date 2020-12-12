//
// Created by Jan Sobol on 27/11/2020.
//

#ifndef RAYTRACER_MATHUTILS_H
#define RAYTRACER_MATHUTILS_H

#include <glm/vec3.hpp>

inline bool nearZero(glm::vec3 vec) {
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

inline glm::vec3 clampColorWithRatio(glm::vec3 vec) {
    if (vec.r > 1.f || vec.g > 1.f || vec.b > 1.f) {
        return vec / std::max(std::max(vec.r, vec.b), vec.g);
    }
    return vec;
}

inline glm::vec3 deNan(glm::vec3 color) {
    if (color.r != color.r)
        color.r = 0.0;
    if (color.g != color.g)
        color.g = 0.0;
    if (color.b != color.b)
        color.b = 0.0;

    return color;
}

#endif //RAYTRACER_MATHUTILS_H
