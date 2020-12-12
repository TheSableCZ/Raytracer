//
// Created by Jan Sobol on 10/12/2020.
//

#ifndef RAYTRACER_PDF_H
#define RAYTRACER_PDF_H

#include <glm/glm.hpp>
#include "../common/Random.h"

class Pdf {
public:
    virtual float value(const glm::vec3 &direction) const = 0;
    virtual glm::vec3 generate() const = 0;
};

class CosinePdf : public Pdf {
public:
    explicit CosinePdf(const glm::vec3 &n) : normal(n) {}

    float value(const glm::vec3 &direction) const override {
        auto cosine = dot(normalize(direction), normal);
        return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
    }

    glm::vec3 generate() const override {
        return calculateRandomDirectionInHemisphere(normal);
    }

    glm::vec3 normal;
};

class MinusCosinePdf : public Pdf {
public:
    explicit MinusCosinePdf(const glm::vec3 &n) : normal(n) {}

    float value(const glm::vec3 &direction) const override {
        auto cosine = dot(-normalize(direction), normal);
        return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
    }

    glm::vec3 generate() const override {
        return -calculateRandomDirectionInHemisphere(normal);
    }

    glm::vec3 normal;
};

#endif //RAYTRACER_PDF_H
