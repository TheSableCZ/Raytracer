//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include <glm/glm.hpp>

struct Ray {
    Ray() = default;
    Ray(glm::vec3 o, glm::vec3 d) : origin(o), direction(d) {}
    glm::vec3 at(float t) const { return origin + t*direction; }
    glm::vec3 origin;
    glm::vec3 direction;
    int colorComponent = -1;
};


#endif //RAYTRACER_RAY_H
