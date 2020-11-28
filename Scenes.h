//
// Created by Jan Sobol on 26/11/2020.
//

#ifndef RAYTRACER_SCENES_H
#define RAYTRACER_SCENES_H

#include "scene/SceneMgr.h"
#include "materials/Metal.h"
#include "scene/objects/Sphere.h"
#include "scene/objects/Polygon.h"
#include "materials/Dielectric.h"
#include "lib/imgui/imgui.h"
#include "materials/Lambertian.h"
#include <glm/gtc/type_ptr.hpp>

class Scene {
public:
    virtual void createScene(SceneMgr &scene) = 0;
    virtual void gui(bool &needReset) = 0;
};

class SimpleScene : public Scene {
public:
    void createScene(SceneMgr &scene) override {
        light = std::make_shared<SimpleMat>(glm::vec3(4));
        metal = std::make_shared<Metal>(glm::vec3 (0.7, 0.6, 0.5), 0.3f);
        auto glass = std::make_shared<Dielectric>(1.5);
        auto lambertian = std::make_shared<Lambertian>(glm::vec3(1.f, 0.f, 0.f));
        auto lambertianBlue = std::make_shared<Lambertian>(glm::vec3(0.f, 0.5f, 0.7f));
        auto lambertianGreen = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.9f, 0.f));
        auto lambertianWhite = std::make_shared<Lambertian>(glm::vec3(1.f, 1.f, 1.f));

        auto sphere1 = std::make_shared<Sphere>(glm::vec3(-100,90,190), 90, lambertianBlue);
        scene.addSceneObject(sphere1);

        scene.addSceneObject(std::make_shared<Sphere>(glm::vec3(400, 90, 190), 90, lambertianGreen));

        /*auto matrix = glm::scale(
                    glm::translate( glm::mat4(1.f), glm::vec3(190,180,190)),
                    glm::vec3(90)
                );

        scene.addSceneObject(std::make_shared<Polygon>(
                glm::vec3(matrix * glm::vec4(-1, 0, 1, 1)),
                glm::vec3(matrix * glm::vec4 (-1, 0, 0, 1)),
                glm::vec3(matrix * glm::vec4 (-1, 1, 0, 1)),
                light
                ));*/
        auto trg = std::make_shared<Polygon>(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), light);
        trg->scale(glm::vec3(180));
        trg->translate(glm::vec3(400,180,300));
        //trg->scale(glm::vec3(90));

        scene.addSceneObject(trg);

        trg = std::make_shared<Polygon>(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), light);
        trg->scale(glm::vec3(180));
        trg->translate(glm::vec3(120,180,300));
        //trg->scale(glm::vec3(90));

        scene.addSceneObject(trg);

        trg = std::make_shared<Polygon>(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), lambertian);
        trg->scale(glm::vec3 (1000, 1000, 10000));
        trg->translate(glm::vec3 (-300, 0, 0));

        scene.addSceneObject(trg);
    }

    void gui(bool &needReset) override {
        ImGui::Begin("Simple scene");
        bool fuzzyMetal = metal->fuzz > 0;
        if (ImGui::Checkbox("Fuzzy metal material", &fuzzyMetal)) {
            metal->fuzz = fuzzyMetal ? 0.4f : 0.f;
            needReset = true;
        }

        if (ImGui::ColorEdit3("Light color", glm::value_ptr(light->color))) needReset = true;
        ImGui::End();
    }

private:
    std::shared_ptr<Metal> metal;
    std::shared_ptr<SimpleMat> light;
};

#endif //RAYTRACER_SCENES_H
