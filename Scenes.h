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
#include "scene/objects/Plane.h"
#include "materials/BSSRDF.h"
#include "scene/objects/ObjLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include "scene/accelerationDS/AABBSimpleADS.h"
#include "scene/accelerationDS/OctreeADS.h"
#include "scene/accelerationDS/AABBBVH.h"

class Scene {
public:
    virtual void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) = 0;

    virtual void gui(bool &needReset) = 0;
};

class SimpleScene : public Scene {
public:
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        /* DEPRECATED
         *
        light = std::make_shared<SimpleMat>(glm::vec3(4));
        metal = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.3f);
        auto glass = std::make_shared<Dielectric>(1.5);
        auto lambertian = std::make_shared<Lambertian>(glm::vec3(1.f, 0.f, 0.f));
        auto lambertianBlue = std::make_shared<Lambertian>(glm::vec3(0.f, 0.5f, 0.7f));
        auto lambertianGreen = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.9f, 0.f));
        auto lambertianWhite = std::make_shared<Lambertian>(glm::vec3(1.f, 1.f, 1.f));

        auto sphere1 = std::make_shared<Sphere>(glm::vec3(-100, 90, 190), 90, lambertianBlue);
        scene.addChild(sphere1);

        scene.addChild(std::make_shared<Sphere>(glm::vec3(400, 90, 190), 90, lambertianGreen));

        auto trg = std::make_shared<Polygon>(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), light);
        trg->scale(glm::vec3(180));
        trg->translate(glm::vec3(400, 180, 300));
        //trg->scale(glm::vec3(90));

        scene.addChild(trg);

        trg = std::make_shared<Polygon>(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), light);
        trg->scale(glm::vec3(180));
        trg->translate(glm::vec3(120, 180, 300));
        //trg->scale(glm::vec3(90));

        scene.addChild(trg);

        trg = std::make_shared<Polygon>(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), lambertian);
        trg->scale(glm::vec3(1000, 1000, 10000));
        trg->translate(glm::vec3(-300, 0, 0));

        scene.addChild(trg);

        trg = std::make_shared<Polygon>(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), lambertianBlue);
        trg->scale(glm::vec3(1000, 10000, 1000));
        trg->translate(glm::vec3(700, 0, 0));

        scene.addChild(trg);*/
    }

    void gui(bool &needReset) override {
        /* DEPRECATED
         *
        ImGui::Begin("Simple scene");
        bool fuzzyMetal = metal->fuzz > 0;
        if (ImGui::Checkbox("Fuzzy metal material", &fuzzyMetal)) {
            metal->fuzz = fuzzyMetal ? 0.4f : 0.f;
            needReset = true;
        }

        if (ImGui::ColorEdit3("Light color", glm::value_ptr(light->color))) needReset = true;
        ImGui::End();*/
    }

private:
    std::shared_ptr<Metal> metal;
    std::shared_ptr<SimpleMat> light;
};

void createCornellBox(SceneMgr &scene) {
    AppSettings::backgroundColor = glm::vec3(0.0f);
    AppSettings::lookfrom = glm::vec3 (278, 278, -800);
    AppSettings::lookat = glm::vec3 (278, 278, 0);
    AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
    scene.camera().init();

    auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
    auto light = std::make_shared<SimpleMat>(glm::vec3(15));
    light->isLightSource = true;

    auto lightObj = createRect(light, glm::vec3(213, 554, 227), glm::vec3(343, 554, 227), glm::vec3(343, 554, 332),glm::vec3(213, 554, 332));
    scene.addChild(lightObj);

    scene.addChild(createRect(green, glm::vec3(555, 0, 0), glm::vec3(555, 0, 555), glm::vec3(555, 555, 555),glm::vec3(555, 555, 0)));
    scene.addChild(createRect(red, glm::vec3(0, 0, 0), glm::vec3(0, 0, 555), glm::vec3(0, 555, 555),glm::vec3(0, 555, 0)));
    scene.addChild(createRect(white, glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(555, 0, 555),glm::vec3(0, 0, 555)));
    scene.addChild(createRect(white, glm::vec3(0, 555, 0), glm::vec3(555, 555, 0), glm::vec3(555, 555, 555),glm::vec3(0, 555, 555)));
    scene.addChild(createRect(white, glm::vec3(0, 0, 555), glm::vec3(555, 0, 555), glm::vec3(555, 555, 555),glm::vec3(0, 555, 555)));

    // neefektivní
    //scene.addChild(std::make_shared<Plane>(glm::vec3 (-1, 0, 0), glm::vec3 (555, 0, 0), green));
    //scene.addChild(std::make_shared<Plane>(glm::vec3 (1, 0, 0), glm::vec3 (0, 0, 0), red));
    //scene.addChild(std::make_shared<Plane>(glm::vec3(0, -1, 0), glm::vec3(0, 555, 0), white));
    //scene.addChild(std::make_shared<Plane>(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), white));
    //scene.addChild(std::make_shared<Plane>(glm::vec3(0, 0, 1), glm::vec3(0, 0, 555), white));
}

void ApplyACTechnique(SceneMgr &scene, int technique, int bvh_leaf_node_capacity) {
    if (technique == 1) { // AABBSimpleADS
        for (const auto &child : scene.getChildren()) {
            if (!child->isLeafNode() && !child->hasAccelerationDS()) {
                child->setAccelerationDS(std::make_unique<LinearADS>());
            }
        }
        scene.setAccelerationDS(std::make_unique<AABBSimpleADS>());
        scene.prepare();
    } else if (technique == 2) {
        scene.setAccelerationDS(std::make_unique<OctreeADS>());
        scene.prepare();

        static_cast<OctreeADS &>(scene.getADSRef()).stats();
    } else if (technique == 3) {
        scene.setAccelerationDS(std::make_unique<AABBBVH>(bvh_leaf_node_capacity));
        scene.prepare();
    } else {
        scene.prepare();
    }
}

class CornellBox : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        createCornellBox(scene);

        auto bssrdf = std::make_shared<BSSRDF>(glm::vec3(.6f, .6f, 1.f), .02f);
        auto diffuse = std::make_shared<Lambertian>(glm::vec3(.85f, .88f, .98f));

        auto box = createUnitBox(bssrdf);
        auto matrix = glm::scale(
                glm::translate(
                        glm::rotate(glm::mat4(1.f), glm::radians(23.f), glm::vec3(0.f, 1.f, 0.f)),
                        glm::vec3(265,200,300)), /*glm::vec3(265,200,450)),*/
                glm::vec3(165, 400, 165)
        );

        box->transform(matrix);
        scene.addChild(box);
        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);
    }

    void gui(bool &needReset) override {}
};

class CornellBox2 : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        createCornellBox(scene);

        auto bssrdf = std::make_shared<BSSRDF>(glm::vec3(.4f, .1f, 6.f), .2f);
        auto diffuse = std::make_shared<Lambertian>(glm::vec3(.85f, .88f, .98f));
        auto glass = std::make_shared<Dielectric>(1.5);
        auto metal = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.3f);

        scene.addChild(std::make_shared<Sphere>(glm::vec3(400, 230, 300), 80, metal));
        scene.addChild(std::make_shared<Sphere>(glm::vec3(110, 160, 350), 80, diffuse));
        scene.addChild(std::make_shared<Sphere>(glm::vec3(400, 110, 40), 80, bssrdf));
        scene.addChild(std::make_shared<Sphere>(glm::vec3(170, 80, 150), 80, glass));

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);
    }

    void gui(bool &needReset) override {}
};

class MaterialScene : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        AppSettings::backgroundColor = glm::vec3(0.7f, 0.8f, 1.f);
        AppSettings::lookfrom = glm::vec3(0, 15, 50);
        AppSettings::lookat = glm::vec3(0, 5, 0);
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        AppSettings::aperture = 0.f;
        scene.camera().init();

        auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
        auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
        auto light = std::make_shared<SimpleMat>(glm::vec3(15, 15, 15));
        light->isLightSource = true;
        auto glass = std::make_shared<Dielectric>(1.5);
        //auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1, 1, 1), glm::vec3(1.f, 0.2f, 0.1f));
        auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1, 1, 1), glm::vec3(1/1.f, 1/0.2f, 1/0.1f));
        //auto bssrdf = std::make_shared<BSSRDF>(glm::vec3(1, 1, 1), 6.f);
        auto white2 = std::make_shared<Lambertian>(glm::vec3(1, 1, 1));

        scene.addChild(std::make_shared<Plane>(glm::vec3(0, 1.f, 0), glm::vec3(0), white));
        auto lightObj = createRect(light, glm::vec3(-10, 20, -5), glm::vec3(10, 20, -5), glm::vec3(10, 20, 5),
                                   glm::vec3(-10, 20, 5));
        // lightObj->makeLight();
        scene.addChild(lightObj);

        //scene.addChild(std::make_shared<Sphere>(glm::vec3(-10, 5, 0), 5, glass));
        scene.addChild(std::make_shared<Sphere>(glm::vec3(10, 5, 0), 5, bssrdf));
        auto box = createUnitBox(bssrdf);
        //box->applyMatrixTransformation(glm::rotate(glm::mat4(1.f), glm::radians(70.f), glm::vec3(0.f, 1.f, 0.f)));
        auto matrix = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(-10, 5, 0)), glm::vec3(10.f));
        box->transform(matrix);
        scene.addChild(box);

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);
    }

    void gui(bool &needReset) override {}
};

class BlenderTest : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        AppSettings::backgroundColor = glm::vec3(0.051f, 0.051f, 0.051f);
        AppSettings::lookfrom = glm::vec3(-6.92579, 4.95831, 7.35889);
        AppSettings::lookat = glm::vec3(0, 0, 0);
        AppSettings::aperture = 0.f;
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        scene.camera().init();

        auto light = std::make_shared<SimpleMat>(glm::vec3(500));
        light->isLightSource = true;
        auto lightRect = createRect(light);
        // lightRect->makeLight();
        auto mat = glm::scale(
                        glm::translate(glm::mat4(1.f), glm::vec3(1.00545, 5.90386, 4.07625)),
                glm::vec3 (1.f));
        auto rotate = glm::rotate(
                glm::rotate(
                glm::rotate(mat, glm::radians(37.261f), glm::vec3(0.f, 0.f, 1.f)),
                glm::radians(3.16371f), glm::vec3 (1.f, 0.f, 0.f)),
                glm::radians(106.936f), glm::vec3 (0.f, 1.f, 0.f));

        // auto matrix = glm::scale(glm::mat4(1.f), glm::vec3(3.f));
        lightRect->transform(rotate);

        scene.addChild(lightRect);

        auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1, 1, 1), glm::vec3(1/1.f, 1/0.2f, 1/0.1f));
        auto box = createUnitBox(bssrdf);

        auto matrix = glm::scale(glm::mat4(1.f), glm::vec3(3.f));
        box->transform(matrix);

        scene.addChild(box);

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);
    }

    void gui(bool &needReset) override {}
};


class LightedCube : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        AppSettings::lookfrom = glm::vec3(3, 3, 3);
        // AppSettings::lookfrom = glm::vec3(-4, 1, 4);
        //AppSettings::lookfrom = glm::vec3(-9.92579, 10, 10.35889);
        AppSettings::lookat = glm::vec3(0, 1.5, 0.5);
        AppSettings::aperture = 0.f;
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        AppSettings::backgroundColor = glm::vec3 (0.f);

        scene.camera().init();

        auto whiteMat = std::make_shared<Lambertian>(glm::vec3(.9, .9, .9));
        auto orangeMat = std::make_shared<Lambertian>(glm::vec3(0.8, .2, .01));
        auto lightMat = std::make_shared<SimpleMat>(glm::vec3(500));
        lightMat->isLightSource = true;

        auto meshes = ObjLoader::loadFromFile(RESOURCE_CUBESCENEDEBUG, whiteMat, {
            {"Material.001", lightMat},
            {"Material.003", orangeMat},
        });

        scene.addChildren(meshes);
        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);
    }

    void gui(bool &needReset) override {}
};


class ObjTest : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        //AppSettings::lookfrom = glm::vec3(-6.92579, 4.95831, 7.35889);
        //AppSettings::lookfrom = glm::vec3(-3, 3, 3);
        AppSettings::lookfrom = glm::vec3(1.2, 2.2, 3.7);
        // AppSettings::lookfrom = glm::vec3(-4, 1, 4);
        //AppSettings::lookfrom = glm::vec3(-9.92579, 10, 10.35889);
        AppSettings::lookat = glm::vec3(-0.2, 0.5, 0);
        AppSettings::aperture = 0.f;
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        AppSettings::backgroundColor = glm::vec3 (0.f);

        scene.camera().init();

        //auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
        //auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
        //auto glass = std::make_shared<Dielectric>(1.5);
        //auto metal = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 1.f), 0.3f);
        //auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1, 1, 1), glm::vec3(1/1.f, 1/0.2f, 1/0.1f));
        auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (.2f, .8f, .2f), glm::vec3(0.02f));

        //std::unordered_map<std::string, std::shared_ptr<Material>> mats;
        //mats.insert(std::pair("glass", glass));
        //mats.insert(std::pair("cruiser", bssrdf));
        //mats.insert(std::pair("metal", metal));

        //auto meshes = ObjLoader::loadFromFile("cruiser.obj", red, mats);
        // auto bssrdf = std::make_shared<BSSRDF>(glm::vec3(.6f, 1.0f, 0.3f), 0.5f);
        // auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1.0f, .7f, 0.5f), 1/0.5f);

        auto redMat = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
        auto orangeMat = std::make_shared<Lambertian>(glm::vec3(0.3, .3, .6));
        auto lightMat = std::make_shared<SimpleMat>(glm::vec3(500));
        lightMat->isLightSource = true;

        auto meshes = ObjLoader::loadFromFile(RESOURCE_UNTITLED2, bssrdf, {
            {"Material.001", lightMat},
            {"Material.003", orangeMat},
        });

        scene.addChildren(meshes);

        //auto light = std::make_shared<SimpleMat>(glm::vec3(15, 15, 15));
        //auto lightObj = createRect(light, glm::vec3(-10, 20, -5), glm::vec3(10, 20, -5), glm::vec3(10, 20, 5),
        //                           glm::vec3(-10, 20, 5));
        //lightObj->lightSource = true;
        //scene.addSceneObject(lightObj);

        //scene.addSceneObject(std::make_shared<Plane>(glm::vec3(0, 1.f, 0), glm::vec3(0, -1.f, 0), white));

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);

    }

    void gui(bool &needReset) override {}
};

class Bunny : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        AppSettings::lookfrom = glm::vec3(3, 3, 3);
        AppSettings::lookat = glm::vec3(0, 1, 0.2);
        AppSettings::aperture = 0.f;
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        AppSettings::backgroundColor = glm::vec3 (0.f);

        scene.camera().init();

        //auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
        //auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
        auto glass = std::make_shared<Dielectric>(1.5);
        //auto metal = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 1.f), 0.3f);
        //auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (1, 1, 1), glm::vec3(1/1.f, 1/0.2f, 1/0.1f));
        // auto bssrdf = std::make_shared<BSSRDF>(glm::vec3 (.2f, .8f, .2f), glm::vec3(0.02f));

        auto redMat = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
        auto orangeMat = std::make_shared<Lambertian>(glm::vec3(0.3, .3, .6));
        auto lightMat = std::make_shared<SimpleMat>(glm::vec3(500));
        lightMat->isLightSource = true;

        auto meshes = ObjLoader::loadFromFile(RESOURCE_BUNNY_SCENE, glass, {
                {"Material.001", lightMat},
                {"Material.003", orangeMat},
        });

        scene.addChildren(meshes);

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);

    }

    void gui(bool &needReset) override {}
};

class Dyno : public Scene {
    void createScene(SceneMgr &scene, int current_ac_technique, int bvh_leaf_node_capacity) override {
        AppSettings::lookfrom = glm::vec3(26.0, 1.5, -4.0);
        AppSettings::lookat = glm::vec3(2, 7.0, -4.5);
        AppSettings::vfov = 33.0f;
        AppSettings::aperture = 0.f;
        AppSettings::distToFocus = glm::length(AppSettings::lookfrom-AppSettings::lookat);
        AppSettings::backgroundColor = glm::vec3 (0.f);

        scene.camera().init();

        auto groundMat = std::make_shared<Lambertian>(glm::vec3(0.000000, 0.010018, 0.044434));
        auto walllMat = std::make_shared<Lambertian>(glm::vec3(0.538225, 0.482123, 0.513181));
        auto dynoMat = std::make_shared<Lambertian>(glm::vec3(0.345588/3.0, 0.007566/3.0, 0.012217/3.0));
        auto lampMat = std::make_shared<Metal>(glm::vec3(0.595588, 0.595588, 0.595588), 0.5);
        auto lightMat = std::make_shared<SimpleMat>(glm::vec3(500));
        lightMat->isLightSource = true;

        auto meshes = ObjLoader::loadFromFile(RESOURCE_DYNO, walllMat, {
                {"groundMat", groundMat},
                {"dynoMat", dynoMat},
                {"lampMat", lampMat},
                {"lightMat", lightMat},
        });

        scene.addChildren(meshes);

        ApplyACTechnique(scene, current_ac_technique, bvh_leaf_node_capacity);

    }

    void gui(bool &needReset) override {}
};


#endif //RAYTRACER_SCENES_H
