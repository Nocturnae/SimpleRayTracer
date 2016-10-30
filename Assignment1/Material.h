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
    Color Calculate(Vector3 intensity, float nlDot, float nhDot) {
        /*
        // ambient
        Vector3 ambientVector = _ambient * ambientIntensity;
        Color ambientColor(ambientVector[0], ambientVector[1], ambientVector[2]);*/
        
        // diffuse
        Vector3 diffuseVector = _diffuse * intensity * nlDot;
        Color diffuseColor(diffuseVector[0], diffuseVector[1], diffuseVector[2]);
        /*
        // blinn-phong
        Vector3 bpVector = _specular.rgb * intensity * pow(nhDot, _specular.phong);
        Color bpColor(bpVector[0], bpVector[1], bpVector[2]);
        
        return diffuseColor + bpColor;*/
        return diffuseColor;
    }

    friend std::istream& operator>>(std::istream& stream, Material& mat);
    
    Material(MaterialId mid, Vector3 ambient, Vector3 diffuse, Vector3 rgb, float phong, Vector3 reflectance) : _materialID(mid), _ambient(ambient), _diffuse(diffuse), _reflectance(reflectance) {
        _specular.rgb = rgb;
        _specular.phong = phong;
    }
    
    MaterialId MaterialID() const {
        return _materialID;
    }
    
    Vector3 Ambient() const {
        return _ambient;
    }
    
    Vector3 Diffuse() const {
        return _diffuse;
    }
};

#endif //RAYTRACER_MATERIAL_H
