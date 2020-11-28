//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_APPSETTINGS_H
#define RAYTRACER_APPSETTINGS_H

#include <glm/vec3.hpp>

class AppSettings {
public:
    static int maxDepth;
    static glm::vec3 backgroundColor;
    static float tMax;
    static float tMin;
    static int imgWidth;
    static int imgHeight;
    static int samplesPerPixel;
    static bool antialiasing;

    // default camera
    static glm::vec3 lookfrom;
    static glm::vec3 lookat;
    static glm::vec3 vup;
    static float distToFocus;
    static float aperture;
    static float vfov;
    static float time0;
    static float time1;
};

#endif //RAYTRACER_APPSETTINGS_H
