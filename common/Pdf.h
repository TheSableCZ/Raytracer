//
// Created by Jan Sobol on 10/12/2020.
//

#ifndef RAYTRACER_PDF_H
#define RAYTRACER_PDF_H

#include <glm/glm.hpp>
#include "../common/Random.h"
#include <random>
#include <ctime>

class Pdf {
public:
    virtual float value(const glm::vec3 &direction, const glm::vec3 &origin) const = 0;
    virtual glm::vec3 generate(const glm::vec3 &origin) const = 0;
};

class CosinePdf : public Pdf {
public:
    explicit CosinePdf(const glm::vec3 &n) : normal(n) {}

    float value(const glm::vec3 &direction, const glm::vec3 &origin) const override {
        auto cosine = dot(normalize(direction), normal);
        return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
    }

    glm::vec3 generate(const glm::vec3 &origin) const override {
        return calculateRandomDirectionInHemisphere(normal);
    }

    glm::vec3 normal;
};

class MinusCosinePdf : public Pdf {
public:
    explicit MinusCosinePdf(const glm::vec3 &n) : normal(n) {}

    float value(const glm::vec3 &direction, const glm::vec3 &origin) const override {
        auto cosine = dot(-normalize(direction), normal);
        return (cosine <= 0) ? 0 : cosine/static_cast<float>(M_PI);
    }

    glm::vec3 generate(const glm::vec3 &origin) const override {
        return -calculateRandomDirectionInHemisphere(normal);
    }

    glm::vec3 normal;
};

class SceneObjsPdf : public Pdf {
public:
    SceneObjsPdf(const std::vector<std::shared_ptr<SceneObject>> &objects, std::unique_ptr<AccelerationDS> accelerationDS = nullptr)
        : objects(objects), accelerationDS(std::move(accelerationDS)) {}

    float value(const glm::vec3 &direction, const glm::vec3 &origin) const override {
        if (accelerationDS == nullptr) {
            auto weight = 1.0 / objects.size();
            auto sum = 0.0;

            for (const auto &polygon : objects)
                sum += weight * polygon->pdfValue(origin, direction);

            return sum;
        } else {
            return accelerationDS->pdfValue(origin, direction) / objects.size();
        }
    }

    glm::vec3 generate(const glm::vec3 &origin) const override {
        static std::random_device rd;
        static std::mt19937 rnd(rd());

        auto int_size = static_cast<int>(objects.size());
        if (int_size > 0) {
            //auto randIdx = randomInt(0, int_size - 1);

            std::uniform_int_distribution<> dist(0, int_size - 1);
            unsigned randIdx = dist(rnd);
            return objects[randIdx]->randomDirection(origin);
        }
        return glm::vec3 (0.f);
    }

    void prepare() {
        if (accelerationDS != nullptr) {
            accelerationDS->buildOver(objects);
        }
    }

private:
    std::vector<std::shared_ptr<SceneObject>> objects;
    std::unique_ptr<AccelerationDS> accelerationDS;
    //glm::vec3 origin;
};

class MixturePdf : public Pdf {
public:
    MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    float value(const glm::vec3 &direction, const glm::vec3 &origin) const override {
        return 0.5 * p[0]->value(direction, origin) + 0.5 * p[1]->value(direction, origin);
    }

    glm::vec3 generate(const glm::vec3 &origin) const override {
        if (randomFloat() < 0.5)
            return p[0]->generate(origin);
        else
            return p[1]->generate(origin);
    }

public:
    std::shared_ptr<Pdf> p[2];
};

#endif //RAYTRACER_PDF_H
