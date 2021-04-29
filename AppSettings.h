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
    static bool useMC;
    static bool lightsDirectSampling;

    // default camera
    static glm::vec3 lookfrom;
    static glm::vec3 lookat;
    static glm::vec3 vup;
    static float distToFocus;
    static float aperture;
    static float vfov;

    // debug modes
    static bool debug_showNormals;
    static bool debug_showFrontSides;
    static bool debug_sampleOnlyLights;

    static int defaultScene;
    static int defaultAccelerationDS;
    static int samplesLimit;

    static int treeLevelMax;
    static int treeLeafLimit;
};

#endif //RAYTRACER_APPSETTINGS_H
