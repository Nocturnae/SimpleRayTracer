#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Color.h"
#include "PointLight.h"
#include "Vertex.h"
#include <iostream>

class Scene {

    std::vector<Vertex> _vertices;

    std::vector<Camera> _cameras;
    std::vector<Sphere> _spheres;
    std::vector<Mesh>   _meshes;

    std::vector<Material> _materials;

    std::vector<PointLight> _lights;

    int _rayReflect;
    Color _background;
    Color _ambient;

public:

    const std::vector<Camera>& Cameras() const {
        return _cameras;
    }

    //////////////////////////////
    // READ SCENE FUNCTIONS HERE !
    //////////////////////////////
    
    void setRayReflect(int rayReflectNum) {
        _rayReflect = rayReflectNum;
    }
    
    void setBackground(Color background) {
        _background = background;
    }
    
    void setAmbient(Color ambient) {
        _ambient = ambient;
    }
    
    void setPointLights(std::vector<PointLight> pointLights) {
        int size = pointLights.size();
        for (int i = 0; i < size; i++) _lights.push_back(pointLights[i]);
    }
    
    
};

Scene* CurrentScene;

void ReadScene(int argc, char** argv)
{
    /*
     * Reads the scene and cameras from arguments
     * Then sets the CurrentScene pointer
     */
    
    std::ifstream scene_file, camera_file;
    scene_file.open(argv[1]);
    camera_file.open(argv[2]);
    
    // SCENE CREATION
    
    CurrentScene = new Scene();
    
    // ray reflection count
    int rayReflectNum;
    std::cin >> rayReflectNum;
    CurrentScene->setRayReflect(rayReflectNum);
    
    // background color
    int background_x, background_y, background_z;
    std::cin >> background_x >> background_y >> background_z;
    Color backgroundColor(background_x, background_y, background_z);
    CurrentScene->setBackground(backgroundColor);
    
    // ambient light
    int ambient_x, ambient_y, ambient_z;
    std::cin >> ambient_x >> ambient_y >> ambient_z;
    Color ambientLight(ambient_x, ambient_y, ambient_z);
    CurrentScene->setAmbient(ambientLight);
    
    // point light
    int pointLightCount;
    std::cin >> pointLightCount;
    
    std::vector<PointLight> pointLightList;
    for (int i = 0; i < pointLightCount; i++) {
        Vector3 points, intensity;
        std::cin >> points;
        std::cin >> intensity;
        PointLight pointLight(points, intensity);
        pointLightList.push_back(pointLight);
    }
    
    CurrentScene->setPointLights(pointLightList);
    
    // material
    int materialCount;
    std::cin >> materialCount;
    
    std::vector<Material> materialList;
    for (int i = 0; i < materialCount; i++) {
        int mid;
        std::cin >> mid;
        
        //
    }
    
    scene_file.close();
}

#endif //RAYTRACER_SCENE_H
