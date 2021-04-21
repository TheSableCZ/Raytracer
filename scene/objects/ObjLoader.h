//
// Created by Jan Sobol on 19/04/2021.
//

#ifndef RAYTRACER_OBJLOADER_H
#define RAYTRACER_OBJLOADER_H

#include <string>
#include <unordered_map>
#include "Polygon.h"
#include "../../lib/OBJ-Loader/Source/OBJ_Loader.h"

class ObjLoader {
public:
    static std::vector<std::shared_ptr<PolygonMesh>> loadFromFile(
            const std::string &filename,
            const std::shared_ptr<Material> &defaultMat,
            std::unordered_map<std::string, std::shared_ptr<Material>> materials = {}
            );

    static glm::vec3 vec3ToGlm3(objl::Vector3 vec) {
        return glm::vec3(vec.X, vec.Y, vec.Z);
    }
};

std::vector<std::shared_ptr<PolygonMesh>> ObjLoader::loadFromFile(const std::string &filename, const std::shared_ptr<Material> &defaultMat, std::unordered_map<std::string, std::shared_ptr<Material>> materials) {
    // Initialize Loader
    objl::Loader Loader;

    std::vector<std::shared_ptr<PolygonMesh>> result;

    // Load .obj File
    bool loaded = Loader.LoadFile(filename);
    if (loaded) {

        // Go through each loaded mesh and out its contents
        for (objl::Mesh &curMesh : Loader.LoadedMeshes) {
            auto found = materials.find(curMesh.MeshMaterial.name);
            std::shared_ptr<Material> mat;
            if (found != materials.end()) {
                mat = found->second;
            } else {
                mat = defaultMat;
            }

            auto polyMesh = std::make_shared<PolygonMesh>(mat);

            for (int i = 0; i < curMesh.Indices.size(); i += 3) {
                int j1 = curMesh.Indices[i], j2 = curMesh.Indices[i+1], j3 = curMesh.Indices[i+2];
                auto p1 = Vertex(vec3ToGlm3(curMesh.Vertices[j1].Position), vec3ToGlm3(curMesh.Vertices[j1].Normal));
                auto p2 = Vertex(vec3ToGlm3(curMesh.Vertices[j2].Position), vec3ToGlm3(curMesh.Vertices[j2].Normal));
                auto p3 = Vertex(vec3ToGlm3(curMesh.Vertices[j3].Position), vec3ToGlm3(curMesh.Vertices[j3].Normal));
                auto poly = std::make_shared<Polygon>(p1, p2, p3);
                polyMesh->add(poly);
            }

            result.emplace_back(polyMesh);
        }

    } else {
        throw std::runtime_error("File " + filename + " can't be open.");
    }

    return result;
}


#endif //RAYTRACER_OBJLOADER_H
