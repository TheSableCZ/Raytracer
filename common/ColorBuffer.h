//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_COLORBUFFER_H
#define RAYTRACER_COLORBUFFER_H

#include <vector>
#include <glm/vec3.hpp>
#include <fstream>

struct ColorBuffer {
    ColorBuffer(int width, int height) : width(width), height(height) {
        p.resize(width * height);
    }

    void saveToPPM() {
        std::ofstream file;
        file.open("image.ppm");
        file << "P3\n" << width << " " << height << "\n255\n";
        for (int j =  height - 1; j >= 0 ; j--)
        {
            for (int i = 0; i < width; ++i)
            {
                file << int(255.99 * p[j*width + i].r) << " " << int(255.99 * p[j*width + i].g) << " " << int(255.99 * p[j*width + i].b) << "\n";
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
