#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Ray.h"
#include "Vertex.h"
#include <cmath>

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

inline bool Sphere::Intersect(const Ray& ray, RayHitInfo& hitInfo) const {
    
    // A = (d.d)/2, B = d.(e-c), B^2 = (d.(e-c))^2, 4AC = (d.d)((e-c).(e-c)-R^2)
    // TODO: check calculations
    Vector3 rayDirection = ray.Direction();
    Vector3 rayOrigin = ray.Origin();
    Vector3 sphereCenter = _center.Position();
    Vector3 ec = rayOrigin - sphereCenter;
    float dd = rayDirection.dotProduct(rayDirection);
    float A = dd / 2;
    float B = rayDirection.dotProduct(ec);
    float B2 = pow(B, 2);
    float AC4 = dd * (ec.dotProduct(ec) - pow(_radius, 2));
    float delta = B2 - AC4;
    
    if (delta <= 0) return false;
    else {
        float t1 = (-B - sqrt(delta)) / (2 * A);
        float t2 = (-B + sqrt(delta)) / (2 * A);
        float t;
        
        if (t1 <= t2) t = t1;
        else t = t2;
        
        Vector3 p = rayOrigin + (rayDirection * t);
        
        hitInfo.Material = _material.MaterialID();
        hitInfo.Position = p;
        hitInfo.Normal = (p - sphereCenter) / _radius;
        //hitInfo.Normal = (p - sphereCenter);
        //hitInfo.Parameter = TODO
        
        return true;
    }
}

#endif //RAYTRACER_SPHERE_H
