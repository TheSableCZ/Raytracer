//
// Created by Jan Sobol on 25/11/2020.
//

#include "Polygon.h"
#include "../../AppSettings.h"
#include "../../common/Random.h"
#include <glm/gtx/norm.hpp>

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

float Polygon::pdfValue(const glm::vec3 &origin, const glm::vec3 &v) {
    Intersection intersection;
    if (!this->intersect(Ray(origin, v), AppSettings::tMin, AppSettings::tMax, intersection))
        return 0;

    auto area = length(cross(p2-p1, p3-p1)) * 0.5; //(x1-x0)*(z1-z0);
    auto distance_squared = intersection.t * intersection.t * length2(v);
    auto cosine = fabs(dot(v, intersection.normal) / length(v));

    return distance_squared / (cosine * area);
}

glm::vec3 Polygon::randomDirection(const glm::vec3 &origin) const {
    auto a = randomFloat();
    auto b = randomFloat();
    if (a+b >= 1) {
        a = 1-a;
        b = 1-b;
    }

    auto randomPoint = p1 + a*(p2-p1) + b*(p3-p1);
    return randomPoint - origin;
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

float PolygonMesh::pdfValue(const glm::vec3 &origin, const glm::vec3 &v) {
    auto weight = 1.0/polygons.size();
    auto sum = 0.0;

    for (const auto &polygon : polygons)
        sum += weight * polygon->pdfValue(origin, v);

    return sum;
}

glm::vec3 PolygonMesh::randomDirection(const glm::vec3 &origin) const {
    auto int_size = static_cast<int>(polygons.size());
    if (int_size > 0) {
        auto randIdx = randomInt(0, int_size - 1);
        return polygons[randIdx]->randomDirection(origin);
    }
    return glm::vec3 (0.f);
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
