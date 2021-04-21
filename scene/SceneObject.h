//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEOBJECT_H
#define RAYTRACER_SCENEOBJECT_H

#include "../common/Ray.h"
#include "../common/Intersection.h"
#include "AccelerationDS.h"

class SceneObject : public std::enable_shared_from_this<SceneObject> {
public:
    SceneObject() = default;
    SceneObject(std::unique_ptr<AccelerationDS> accelerationDS);
    SceneObject(const std::vector<std::shared_ptr<SceneObject>> &children, std::unique_ptr<AccelerationDS> accelerationDS = nullptr);

    virtual ~SceneObject() {}

    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection);
    virtual AABBValue getAABBValue() const;
    virtual float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) { return 0.0; }
    virtual glm::vec3 randomDirection(const glm::vec3 &origin) const { return glm::vec3(0); }
    virtual std::vector<std::shared_ptr<SceneObject>> getLightSources();
    virtual std::vector<std::shared_ptr<SceneObject>> getLeafs();
    virtual void prepare();

    bool isLeafNode() const { return children.empty(); }
    bool isLightSource() { return isLeafNode() && lightSource; }

    void setTransform(const glm::mat4 &matrix) {
        transform = matrix;
    }

    void addChild(const std::shared_ptr<SceneObject>& sceneObj);
    void makeLight();

    bool lightSource = false;

    void setMaterial(const std::shared_ptr<Material> &mat) { this->mat = mat; }

protected:
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector<std::shared_ptr<SceneObject>> children;
    std::unique_ptr<AccelerationDS> accelerationDS;
    std::shared_ptr<Material> mat;
    std::string msg = "";
};

#endif //RAYTRACER_SCENEOBJECT_H
