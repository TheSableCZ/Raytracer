//
// Created by Petr Fusek on 20/04/2021.
//

#include "AABBSimpleADS.h"

bool AABBSimpleADS::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    Intersection closestIntersection;
    auto intersected = false;
    auto closestSoFar = tMax;
    for (auto& node : nodes) {
        if (node.bb.isIntersecting(ray, tMin, closestSoFar)
            && node.obj->intersect(ray, tMin, closestSoFar, closestIntersection)
        ) {
            intersected = true;
            closestSoFar = closestIntersection.t;
            intersection = closestIntersection;
        }
    }
    return intersected;
}
