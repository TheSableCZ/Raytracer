//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include <glm/glm.hpp>
#include "common/Ray.h"
#include "scene/SceneMgr.h"
#include "common/ColorBuffer.h"
#include "common/Camera.h"

class Raytracer {
public:
    Raytracer();
    glm::vec3 trace(const Ray &ray, int depth);
    void render(ColorBuffer &colorBuffer, int width, int height);
    void renderStage(ColorBuffer &colorBuffer, int width, int height);
    static glm::vec3 pixelColorOperation(glm::vec3 pixelColor, int samplesPerPixel);

    void initCameraWithAppSettings();

    SceneMgr& scene() { return sceneMgr; }

private:
    SceneMgr sceneMgr;
    Camera camera; //(AppSettings::lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1)
};


#endif //RAYTRACER_RAYTRACER_H
