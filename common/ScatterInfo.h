//
// Created by Jan Sobol on 10/12/2020.
//

#ifndef RAYTRACER_SCATTERINFO_H
#define RAYTRACER_SCATTERINFO_H

#include "Pdf.h"

enum class ScatteredRayType {
    none,
    BSSRDF_enteringMedium,
    BSSRDF_exitingMedium,
    BSSRDF_insideMedium
};

struct ScatterInfo {
    glm::vec3 attenuation;
    Ray scatteredRay;

    bool useMC = false;
    std::shared_ptr<Pdf> pdfPtr;
    glm::vec3 rayOrigin;

    ScatteredRayType scatteredRayType = ScatteredRayType::none;
    int colorChannel = -1;
};

#endif //RAYTRACER_SCATTERINFO_H
