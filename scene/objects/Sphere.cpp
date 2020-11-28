//
// Created by Jan Sobol on 25/11/2020.
//

#include "Sphere.h"
#include <glm/gtx/norm.hpp>

bool Sphere::intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
    glm::vec3 oc = ray.origin - center;
    auto a = glm::length2(ray.direction);
    auto half_b = dot(oc, ray.direction);
    auto c = glm::length2(oc) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < tMin || tMax < root) {
        root = (-half_b + sqrtd) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    intersection.t = root;
    intersection.point = ray.at(root);
    intersection.setFaceAndNormal(ray.direction, glm::normalize(intersection.point - center));

    //intersection.frontFace = dot(ray.direction, intersection.normal) < 0;
    //intersection.normal = intersection.frontFace ? intersection.normal : -intersection.normal;

    intersection.objectPtr = shared_from_this();
    //rec.set_face_normal(r, outward_normal);
    //get_sphere_uv(outward_normal, rec.u, rec.v);
    //rec.mat_ptr = mat_ptr;

    return true;
}
