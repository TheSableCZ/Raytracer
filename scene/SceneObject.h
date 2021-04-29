//
// Created by Jan Sobol on 25/11/2020.
//

#ifndef RAYTRACER_SCENEOBJECT_H
#define RAYTRACER_SCENEOBJECT_H

#include "../common/Ray.h"
#include "../common/Intersection.h"
#include "AccelerationDS.h"
#include "accelerationDS/LinearADS.h"
#include "../materials/Material.h"

class SceneObject : public std::enable_shared_from_this<SceneObject> {
public:
    SceneObject() = default;
    SceneObject(std::unique_ptr<AccelerationDS> accelerationDS);
    SceneObject(const std::vector<std::shared_ptr<SceneObject>> &children, std::unique_ptr<AccelerationDS> accelerationDS = nullptr);

    template<typename T>
    SceneObject(const std::vector<std::shared_ptr<T>> &children) {
        for (const auto &child : children) {
            addChild(child);
        }
    }

    // virtuals methods
    virtual std::vector<std::shared_ptr<SceneObject>> getLightSources();
    virtual std::vector<std::shared_ptr<SceneObject>> getLeafs();
    virtual void prepare();
    virtual void transform(const glm::mat4 &matrix);
    virtual AABB getAABB() const;

    // virtual inlines methods
    virtual bool intersect(const Ray &ray, float tMin, float tMax, Intersection &intersection) {
        return accelerationDS != nullptr
            ? accelerationDS->intersect(ray, tMin, tMax, intersection)
            : false;
    }
    virtual float pdfValue(const glm::vec3 &origin, const glm::vec3 &v) { return 0.0; }
    virtual glm::vec3 randomDirection(const glm::vec3 &origin) const { return glm::vec3(0); }

    // non-virtual methods
    bool isLeafNode() const { return children.empty(); }
    bool isLightSource() const { return isLeafNode() && mat != nullptr && mat->isLightSource; }
    const std::vector<std::shared_ptr<SceneObject>> &getChildren() { return children; }
    bool hasAccelerationDS() { return accelerationDS != nullptr; }

    void addChild(const std::shared_ptr<SceneObject>& sceneObj);
    void addChildren(const std::vector<std::shared_ptr<SceneObject>>& sceneObjs);
    void setMaterial(const std::shared_ptr<Material> &mat);

    void setAccelerationDS(std::unique_ptr<AccelerationDS> ds) { accelerationDS = std::move(ds); }
    AccelerationDS &getADSRef() { return *accelerationDS; }

protected:
    std::shared_ptr<Material> mat;
    std::unique_ptr<AccelerationDS> accelerationDS;
    std::vector<std::shared_ptr<SceneObject>> children;

};

#endif //RAYTRACER_SCENEOBJECT_H
