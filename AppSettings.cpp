//
// Created by Jan Sobol on 25/11/2020.
//

#include <glm/geometric.hpp>
#include "AppSettings.h"

int AppSettings::maxDepth(50);
glm::vec3 AppSettings::backgroundColor(0.7f, 0.8f, 1.f);
float AppSettings::tMax(std::numeric_limits<float>::infinity());
float AppSettings::tMin(0.001);

int AppSettings::imgWidth(600);
int AppSettings::imgHeight(600);

int AppSettings::samplesPerPixel(10);
bool AppSettings::antialiasing(true);
bool AppSettings::useMC(true);
bool AppSettings::lightsDirectSampling(true);


glm::vec3 AppSettings::lookfrom(0, 0, 10);
glm::vec3 AppSettings::lookat(0,0,0);
glm::vec3 AppSettings::vup(0, 1, 0);
float AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
float AppSettings::aperture = 2.0;
float AppSettings::vfov = 40.0;

bool AppSettings::debug_showNormals = false;
bool AppSettings::debug_showFrontSides = false;
bool AppSettings::debug_sampleOnlyLights = false;
