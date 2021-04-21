//
// Created by Petr Fusek on 20/04/2021.
//

#include "LinearADS.h"

bool LinearADS::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    Intersection closestIntersection;
    auto intersected = false;
    auto closestSoFar = tMax;
    for (const auto& object : objects) {
        if (object->intersect(ray, tMin, closestSoFar, closestIntersection)) {
            intersected = true;
            closestSoFar = closestIntersection.t;
            intersection = closestIntersection;
        }
    }
    return intersected;
}
