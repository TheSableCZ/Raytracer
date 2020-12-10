//
// Created by Jan Sobol on 09/12/2020.
//

#include "Plane.h"

bool Plane::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    float np = dot(n, ray.direction);
    if (np == 0)
        return false;

    float t = -(d + dot(n, ray.origin)) / np;
    if (t >= tMin && t <= tMax) {
    //if (t > 0) {
        intersection.t = t;
        intersection.point = ray.at(t);
        intersection.setFaceAndNormal(ray.direction, n);

        intersection.objectPtr = shared_from_this();
        intersection.materialPtr = mat;

        return true;
    } else return false;
}
