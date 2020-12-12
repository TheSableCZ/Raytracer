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

class SceneObjsPdf : public Pdf {
public:
    SceneObjsPdf(const std::vector<std::shared_ptr<SceneObject>> &objects, const glm::vec3 &origin)
        : objects(objects), origin(origin) {}

    float value(const glm::vec3 &direction) const override {
        auto weight = 1.0/objects.size();
        auto sum = 0.0;

        for (const auto &polygon : objects)
            sum += weight * polygon->pdfValue(origin, direction);

        return sum;
    }

    glm::vec3 generate() const override {
        auto int_size = static_cast<int>(objects.size());
        if (int_size > 0) {
            auto randIdx = randomInt(0, int_size - 1);
            return objects[randIdx]->randomDirection(origin);
        }
        return glm::vec3 (0.f);
    }

    const std::vector<std::shared_ptr<SceneObject>> &objects;
    glm::vec3 origin;
};

class MixturePdf : public Pdf {
public:
    MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    float value(const glm::vec3 &direction) const override {
        return 0.5 * p[0]->value(direction) + 0.5 *p[1]->value(direction);
    }

    glm::vec3 generate() const override {
        if (randomFloat() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

public:
    std::shared_ptr<Pdf> p[2];
};

#endif //RAYTRACER_PDF_H
