#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "Color.h"
#include "Vector3.h"

class Scene;

using MaterialId = int;

class Material {
    //////////////////
    /// PUT YOUR VARIABLES, HELPER FUNCTIONS HERE !
    //////////////////
    MaterialId _materialID;
    Vector3 _ambient;
    Vector3 _diffuse;
    struct {
        Vector3 rgb;
        float phong;
    } _specular;
    Vector3 _reflectance;
public:

    /*
     * Implement a Calculate function that does Diffuse, Specular and Ambient, Reflective shading
     */

    friend std::istream& operator>>(std::istream& stream, Material& mat);
    
    Material(MaterialId mid, Vector3 ambient, Vector3 diffuse, Vector3 rgb, float phong, Vector3 reflectance) : _materialID(mid), _ambient(ambient), _diffuse(diffuse), _reflectance(reflectance) {
        _specular.rgb = rgb;
        _specular.phong = phong;
    }
    
    MaterialId MaterialID() const {
        return _materialID;
    }
};

#endif //RAYTRACER_MATERIAL_H
