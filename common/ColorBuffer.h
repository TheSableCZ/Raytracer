//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_COLORBUFFER_H
#define RAYTRACER_COLORBUFFER_H

#include <vector>
#include <glm/vec3.hpp>
#include <fstream>
#include <functional>

struct ColorBuffer {
    ColorBuffer(int width, int height) : width(width), height(height) {
        p.resize(width * height);
    }

    void saveToPPM(const char * filename, std::function<glm::vec3(const glm::vec3 &)> perPixelOp) {
        std::ofstream file;
        file.open(filename);
        file << "P3\n" << width << " " << height << "\n255\n";
        for (int j =  height - 1; j >= 0 ; j--)
        {
            for (int i = 0; i < width; ++i)
            {
                auto finalColor = perPixelOp(p[j*width + i]);
                file << int(255.99 * finalColor.r) << " " << int(255.99 * finalColor.g) << " " << int(255.99 * finalColor.b) << "\n";
            }
        }
        file.close();
    }

    int size() const { return width * height; }

    void clear() { p.clear(); p.resize(size()); }

    std::vector<glm::vec3> p;
    int width;
    int height;
};

#endif //RAYTRACER_COLORBUFFER_H
