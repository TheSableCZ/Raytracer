//
// Created by Jan Sobol on 25/11/2020.
//

#include "Polygon.h"

bool Polygon::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    const float EPSILON = 0.0000001;

    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v;

    edge1 = p2 - p1;
    edge2 = p3 - p1;
    h = cross(ray.direction, edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.

    f = 1.f/a;
    s = ray.origin - p1;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = cross(s, edge1);
    v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t >= tMin && t <= tMax) // ray intersection
    {
        intersection.t = t;
        intersection.point = ray.at(t);

        intersection.setFaceAndNormal(ray.direction, normalize(cross(edge1, edge2)));

        //intersection.normal = normalize(cross(edge1, edge2));
        //intersection.frontFace = dot(ray.direction, intersection.normal) < 0;
        //intersection.normal = intersection.frontFace ? intersection.normal : -intersection.normal;

        intersection.objectPtr = shared_from_this();
        intersection.materialPtr = mat;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

void Polygon::applyMatrixTransformation(glm::mat4 matrix) {
    glm::vec4 tmp_4;
    tmp_4 = matrix * glm::vec4 (p1, 1.f);
    p1 = glm::vec3 (tmp_4 / tmp_4.w);

    tmp_4 = matrix * glm::vec4 (p2, 1.f);
    p2 = glm::vec3 (tmp_4 / tmp_4.w);

    tmp_4 = matrix * glm::vec4 (p3, 1.f);
    p3 = glm::vec3 (tmp_4 / tmp_4.w);
}

bool PolygonMesh::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    Intersection tempRec;
    auto intersect = false;
    auto closestSoFar = tMax;

    for (const auto& polygon : polygons) {
        if (polygon->intersect(ray, tMin, closestSoFar, tempRec)) {
            intersect = true;
            closestSoFar = tempRec.t;
            intersection = tempRec;
        }
    }

    return intersect;
}

std::shared_ptr<PolygonMesh> createRect(const std::shared_ptr<Material> &mat, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
    std::vector<std::shared_ptr<Polygon>> polygons;
    polygons.emplace_back(std::make_shared<Polygon>(p1, p2, p3, mat));
    polygons.emplace_back(std::make_shared<Polygon>(p3, p4, p1, mat));
    return std::make_shared<PolygonMesh>(polygons);
}

std::shared_ptr<PolygonMesh> createUnitBox(const std::shared_ptr<Material> &mat) {
    glm::vec3   p1 = glm::vec3 (-.5f, -.5f, -.5f),
                p2 = glm::vec3 (.5f, -.5f, -.5f),
                p3 = glm::vec3 (.5f, .5f, -.5f),
                p4 = glm::vec3 (-.5f, .5f, -.5f),
                p5 = glm::vec3 (-.5f, -.5f, .5f),
                p6 = glm::vec3 (.5f, -.5f, .5f),
                p7 = glm::vec3 (.5f, .5f, .5f),
                p8 = glm::vec3 (-.5f, .5f, .5f);

    auto mesh = std::make_shared<PolygonMesh>();
    mesh->add(createRect(mat, p2, p1, p4, p3));
    mesh->add(createRect(mat, p6, p2, p3, p7));
    mesh->add(createRect(mat, p5, p6, p7, p8));
    mesh->add(createRect(mat, p1, p5, p8, p4));
    mesh->add(createRect(mat, p1, p2, p6, p5));
    mesh->add(createRect(mat, p3, p4, p8, p7));

    return mesh;
}
