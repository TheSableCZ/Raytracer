//
// Created by Jan Sobol on 25/11/2020.
//

#include <glm/geometric.hpp>
#include "AppSettings.h"

int AppSettings::maxDepth(50);
//glm::vec3 AppSettings::backgroundColor(0.3f, .3f, .7f);
glm::vec3 AppSettings::backgroundColor(0.f);
float AppSettings::tMax(std::numeric_limits<float>::infinity());
float AppSettings::tMin(0.001);

int AppSettings::imgWidth(600);
int AppSettings::imgHeight(600);

int AppSettings::samplesPerPixel(10);
bool AppSettings::antialiasing(true);
bool AppSettings::useMC(true);

//glm::vec3 AppSettings::lookfrom(-800, 350, 100);
//glm::vec3 AppSettings::lookfrom(120, 800, 301);
glm::vec3 AppSettings::lookfrom(278, 278, -800);
glm::vec3 AppSettings::lookat(278, 278, 0);
//glm::vec3 AppSettings::lookat(400, 90, 190);
//glm::vec3 AppSettings::lookat(120,180,300);
//glm::vec3 AppSettings::lookfrom(0, 0, 10);
//glm::vec3 AppSettings::lookat(0,0,0);
glm::vec3 AppSettings::vup(0, 1, 0);
//float AppSettings::distToFocus = 250.0;
float AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
float AppSettings::aperture = 2.0;
float AppSettings::vfov = 40.0;
float AppSettings::time0 = 0.0;
float AppSettings::time1 = 1.0;

bool AppSettings::debug_showNormals = false;
bool AppSettings::debug_showFrontSides = false;
