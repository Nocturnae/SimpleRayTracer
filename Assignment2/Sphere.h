//
// Created by Fatih on 10/27/15.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Vertex.h"
#include "Material.h"
#include "Ray.h"

class Scene;

class Sphere {
    const Scene * _scene;

    VertexId    _center;

    MaterialId  _material;

    float       _radius;

public:
    Sphere() = default;
    explicit Sphere(VertexId center, float radius, MaterialId material);

    void SetScene(const Scene* scene);

    bool RayHit(const Ray& ray, RayHitInfo& hitInfo) const;

    Vector3 Normal(const Vector3& at) const;

    friend std::istream& operator>>(std::istream& stream, Sphere& sphere);
};


#endif //RAYTRACER_SPHERE_H
