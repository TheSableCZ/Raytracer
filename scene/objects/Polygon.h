//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <glm/gtc/matrix_transform.hpp>
#include "../SceneObject.h"

struct Vertex {
    Vertex(glm::vec3 p, glm::vec3 n) : p(p), n(n) {}
    glm::vec3 p, n;
};

class Polygon : public SceneObject {
public:
    Polygon(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
        : SceneObject(), p1(p1, glm::vec3(0)), p2(p2, glm::vec3(0)), p3(p3, glm::vec3(0)), verticesWithNormal(false)
        { bb = computeAABB(); }

    Polygon(Vertex p1, Vertex p2, Vertex p3)
        : SceneObject(), p1(p1), p2(p2), p3(p3), verticesWithNormal(true)
        { bb = computeAABB(); }

    void translate(glm::vec3 trVec) {
        transform(glm::translate(glm::mat4(1.f), trVec));
    }

    void scale(glm::vec3 scVec) {
        transform(glm::scale(glm::mat4(1.f), scVec));
    }

    AABB computeAABB() const;

    // overrides
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;
    virtual void transform(const glm::mat4 &matrix) override;
    virtual float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) override;
    virtual glm::vec3 randomDirection(const glm::vec3 &origin) const override;
    virtual AABB getAABB() const override;

private:
    Vertex p1;
    Vertex p2;
    Vertex p3;
    bool verticesWithNormal;
    AABB bb;
};

/*
 * Utils functions
 */
std::shared_ptr<SceneObject> createRect(
        const std::shared_ptr<Material> &mat,
        glm::vec3 p1 = glm::vec3 (-.5f, -.5f, 0.f),
        glm::vec3 p2 = glm::vec3 (.5f, -.5f, 0.f),
        glm::vec3 p3 = glm::vec3 (.5f, .5f, 0.f),
        glm::vec3 p4 = glm::vec3 (-.5f, .5f, 0.f)
        );

std::shared_ptr<SceneObject> createUnitBox(const std::shared_ptr<Material> &mat);


#endif //RAYTRACER_POLYGON_H
