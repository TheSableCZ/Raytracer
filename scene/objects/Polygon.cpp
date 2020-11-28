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
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
