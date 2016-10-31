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
    float a_x = _vertices[0].Position()._data[0], b_x = _vertices[1].Position()._data[0], c_x = _vertices[2].Position()._data[0];
    float a_y = _vertices[0].Position()._data[1], b_y = _vertices[1].Position()._data[1], c_y = _vertices[2].Position()._data[1];
    float a_z = _vertices[0].Position()._data[2], b_z = _vertices[1].Position()._data[2], c_z = _vertices[2].Position()._data[2];
    float a = a_x - b_x, b = a_y - b_y, c = a_z - b_z,
            d = a_x - c_x, e = a_y - c_y, f = a_z - c_z,
                g = rayDirection._data[0], h = rayDirection._data[1], i = rayDirection._data[2],
                    j = a_x - rayOrigin._data[0], k = a_y - rayOrigin._data[1], l = a_z - rayOrigin._data[2];
    
    float akjb = (a * k) - (j * b);
    float blkc = (b * l) - (k * c);
    float dheg = (d * h) - (e * g);
    float eihf = (e * i) - (h * f);
    float gfdi = (g * f) - (d * i);
    float jcal = (j * c) - (a * l);
    float detA = (a * eihf) + (b * gfdi) + (c * dheg);
    
    float t = -((f * akjb) + (e * jcal) + (d * blkc)) / detA;
    if (t < 0) return false;
    
    float gamma = ((i * akjb) + (h * jcal) + (g * blkc)) / detA;
    if ((gamma < 0) || (gamma > 1)) return false;
    
    float beta = ((j * eihf) + (k * gfdi) + (l * dheg)) / detA;
    if ((beta < 0) || (beta > (1 - gamma))) return false;

    Vector3 p = rayOrigin + (rayDirection * t);
        
    Vector3 bc(c_x - b_x, c_y - b_y, c_z - b_z);
    Vector3 ba(a_x - b_x, a_y - b_y, a_z - b_z);
        
    hitInfo.Material = _material.MaterialID();
    hitInfo.Position = p;
    hitInfo.Normal = Vector3((bc[1] * ba[2]) - (bc[2] * ba[1]),
                             (bc[2] * ba[0]) - (bc[0] * ba[2]),
                             (bc[0] * ba[1]) - (bc[1] * ba[0]));
    hitInfo.Normal = hitInfo.Normal / hitInfo.Normal.length();
    hitInfo.Parameter = t;
        
    return true;
}

#endif //RAYTRACER_TRIANGLE_H

