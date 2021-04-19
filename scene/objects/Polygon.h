//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_POLYGON_H
#define RAYTRACER_POLYGON_H

#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <vector>
#include "../SceneObject.h"

struct Vertex {
    Vertex(glm::vec3 p, glm::vec3 n) : p(p), n(n) {}
    glm::vec3 p, n;
};

class Polygon : public SceneObject, public std::enable_shared_from_this<Polygon> {
public:
    Polygon(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, std::shared_ptr<Material> mat)
        : SceneObject(), p1(p1, glm::vec3(0)), p2(p2, glm::vec3(0)), p3(p3, glm::vec3(0)), mat(std::move(mat)), verticesWithNormal(false) {}

    Polygon(Vertex p1, Vertex p2, Vertex p3, std::shared_ptr<Material> mat)
        : SceneObject(), p1(p1), p2(p2), p3(p3), mat(std::move(mat)), verticesWithNormal(true) {}

    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;
    void applyMatrixTransformation(glm::mat4 matrix);

    void translate(glm::vec3 trVec) {
        applyMatrixTransformation(glm::translate(glm::mat4(1.f), trVec));
    }

    void scale(glm::vec3 scVec) {
        applyMatrixTransformation(glm::scale(glm::mat4(1.f), scVec));
    }

    float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) override;
    glm::vec3 randomDirection(const glm::vec3 &origin) const override;

    std::shared_ptr<Material> mat;

private:
    Vertex p1;
    Vertex p2;
    Vertex p3;
    bool verticesWithNormal;
};

class PolygonMesh : public SceneObject {
public:
    PolygonMesh() = default;
    explicit PolygonMesh(std::vector<std::shared_ptr<Polygon>> polygonMesh)
        : SceneObject(), polygons(std::move(polygonMesh)) {}

    bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) override;

    void applyMatrixTransformation(glm::mat4 matrix) {
        for(const auto &polygon: polygons)
            polygon->applyMatrixTransformation(matrix);
    }

    void add(const std::shared_ptr<Polygon> &polygon) {
        polygons.emplace_back(polygon);
    }

    void add(const std::shared_ptr<PolygonMesh> &polygonMesh) {
        polygons.insert(polygons.end(), polygonMesh->polygons.begin(), polygonMesh->polygons.end());
    }

    float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) override;
    glm::vec3 randomDirection(const glm::vec3 &origin) const override;

    bool isLightSource() const override { return lightSource; }


    bool lightSource = false;

private:
    std::vector<std::shared_ptr<Polygon>> polygons;
};

/*
 * Utils functions
 */
std::shared_ptr<PolygonMesh> createRect(
        const std::shared_ptr<Material> &mat,
        glm::vec3 p1 = glm::vec3 (-.5f, -.5f, 0.f),
        glm::vec3 p2 = glm::vec3 (.5f, -.5f, 0.f),
        glm::vec3 p3 = glm::vec3 (.5f, .5f, 0.f),
        glm::vec3 p4 = glm::vec3 (-.5f, .5f, 0.f)
        );

std::shared_ptr<PolygonMesh> createUnitBox(const std::shared_ptr<Material> &mat);


#endif //RAYTRACER_POLYGON_H
