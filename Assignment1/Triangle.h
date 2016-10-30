#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Ray.h"

class Triangle
{
    Material _material;
public:
    Vertex _vertices[3];
    Triangle(Material material, Vertex v1, Vertex v2, Vertex v3) : _material(material) {
        _vertices[0] = v1;
        _vertices[1] = v2;
        _vertices[2] = v3;
    }
    // This function gets a ray, and if the triangle intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};

inline bool Triangle::Intersect(const Ray& ray, RayHitInfo& hitInfo) const {
    Vector3 rayDirection = ray.Direction();
    Vector3 rayOrigin = ray.Origin();
    
    // check calculations
    float x_a = _vertices[0].Position()._data[0], x_b = _vertices[1].Position()._data[0], x_c = _vertices[2].Position()._data[0];
    float y_a = _vertices[0].Position()._data[1], y_b = _vertices[1].Position()._data[1], y_c = _vertices[2].Position()._data[1];
    float z_a = _vertices[0].Position()._data[2], z_b = _vertices[1].Position()._data[2], z_c = _vertices[2].Position()._data[2];
    float a = x_a - x_b, b = y_a - y_b, c = z_a - z_b,
            d = x_a - x_c, e = y_a - y_c, f = z_a - z_c,
                g = rayDirection._data[0], h = rayDirection._data[1], i = rayDirection._data[2],
                    j = x_a - rayOrigin._data[0], k = y_a - rayOrigin._data[1], l = z_a - rayOrigin._data[2];
    
    float akjb = (a * k) - (j * b);
    float blkc = (b * l) - (k * c);
    float dheg = (d * h) - (e * g);
    float eihf = (e * i) - (h * f);
    float gfdi = (g * f) - (d * i);
    float jcal = (j * c) - (a * l);
    float detA = (a * eihf) + (b * gfdi) + (c * dheg);
    
    float t = -((f * akjb) + (e * jcal) + (d * blkc)) / detA;
    if (t < 0.0001) return false;
    
    float gamma = ((i * akjb) + (h * jcal) + (g * blkc)) / detA;
    if ((gamma < 0.0001) || (gamma - 1 > 0.0001)) return false;
    
    float beta = ((j * eihf) + (k * gfdi) + (l * dheg)) / detA;
    if ((beta < 0.0001) || (beta > (1 - gamma))) return false;

    Vector3 p = rayOrigin + (rayDirection * t);
        
    Vector3 bc(x_c - x_b, y_c - y_b, z_c - z_b);
    Vector3 ba(x_a - x_b, y_a - y_b, z_a - z_b);
        
    hitInfo.Material = _material.MaterialID();
    hitInfo.Position = p;
    hitInfo.Normal = Vector3((bc._data[1] * ba._data[2]) - (bc._data[2] * ba._data[1]),
                             (bc._data[0] * ba._data[2]) - (bc._data[2] * ba._data[0]),
                             (bc._data[0] * ba._data[1]) - (bc._data[1] * ba._data[0]));
    hitInfo.Normal = hitInfo.Normal / hitInfo.Normal.length();
    hitInfo.Parameter = t;
        
    return true;
}

#endif //RAYTRACER_TRIANGLE_H

