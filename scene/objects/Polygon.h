//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <glm/gtc/matrix_transform.hpp>
#include "../SceneObject.h"

class Polygon : public SceneObject, public std::enable_shared_from_this<Polygon> {
public:
    Polygon(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, std::shared_ptr<Material> mat) : SceneObject(mat), p1(p1), p2(p2), p3(p3) {}
    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

    void translate(glm::vec3 trVec) {
        auto matrix = glm::translate(glm::mat4(1.f), trVec);
        glm::vec4 tmp_4;
        tmp_4 = matrix * glm::vec4 (p1, 1.f);
        p1 = glm::vec3 (tmp_4 / tmp_4.w);

        tmp_4 = matrix * glm::vec4 (p2, 1.f);
        p2 = glm::vec3 (tmp_4 / tmp_4.w);

        tmp_4 = matrix * glm::vec4 (p3, 1.f);
        p3 = glm::vec3 (tmp_4 / tmp_4.w);
    }

    void scale(glm::vec3 scVec) {
        auto matrix = glm::scale(glm::mat4(1.f), scVec);
        glm::vec4 tmp_4;
        tmp_4 = matrix * glm::vec4 (p1, 1.f);
        p1 = glm::vec3 (tmp_4 / tmp_4.w);

        tmp_4 = matrix * glm::vec4 (p2, 1.f);
        p2 = glm::vec3 (tmp_4 / tmp_4.w);

        tmp_4 = matrix * glm::vec4 (p3, 1.f);
        p3 = glm::vec3 (tmp_4 / tmp_4.w);
    }

private:
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
};


#endif //RAYTRACER_POLYGON_H
