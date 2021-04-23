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
#include "common/ScatterInfo.h"

class Raytracer {
public:
    Raytracer();
    glm::vec3 trace(const Ray &ray, int depth, int colorChannel);
    inline glm::vec3 calculateScatteredRay(const Ray &inRay, const Intersection &intersection, const ScatterInfo &scatterInfo,
                                    int depth, int colorChannel = -1);
    void renderStage(ColorBuffer &colorBuffer, int width, int height);
    static glm::vec3 pixelColorOperation(glm::vec3 pixelColor, int samplesPerPixel);

    std::shared_ptr<SceneMgr>& scene() { return sceneMgr; }
    void clearScene() { sceneMgr = std::make_shared<SceneMgr>(); sceneMgr->camera().init(); }

private:
    std::shared_ptr<SceneMgr> sceneMgr;
};


#endif //RAYTRACER_RAYTRACER_H
