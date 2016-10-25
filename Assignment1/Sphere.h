#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Ray.h"
#include "Vertex.h"

class Sphere {
private:
    int _sphereID;
    Material _material;
    float _radius;
    Vertex _center;
public:
    Sphere(int sphereID, Material material, float radius, Vertex center) : _material(material) {
        _sphereID = sphereID;
        _radius = radius;
        _center = center;
    }
    
    // This function gets a ray, and if the sphere intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};


#endif //RAYTRACER_SPHERE_H
