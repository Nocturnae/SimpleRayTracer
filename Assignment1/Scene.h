#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Color.h"
#include "PointLight.h"
#include "Material.h"
#include "Vertex.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Material.h"

class Material;

class Scene {

    //std::vector<Vertex> _vertices;

    std::vector<Camera> _cameras;
    std::vector<Sphere> _spheres;
    std::vector<Mesh>   _meshes;

    std::vector<Material> _materials;

    std::vector<PointLight> _lights;

    int _rayReflect;
    Color _background;
    Color _ambient;

public:
    
    std::vector<Vertex> _vertices;

    const std::vector<Camera>& Cameras() const {
        return _cameras;
    }
    
    const std::vector<Mesh>& Meshes() const {
        return _meshes;
    }
    
    const std::vector<Sphere>& Spheres() const {
        return _spheres;
    }
    
    const std::vector<PointLight>& Lights() const {
        return _lights;
    }
    
    Color Ambient() const {
        return _ambient;
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
    
    Color Background() const {
        return _background;
    }
    
    void setAmbient(Color ambient) {
        _ambient = ambient;
    }
    
    void setPointLights(std::vector<PointLight> pointLights) {
        size_t size = pointLights.size();
        for (int i = 0; i < size; i++) _lights.push_back(pointLights[i]);
    }
    
    void setMaterials(std::vector<Material> materials) {
        size_t size = materials.size();
        for (int i = 0; i < size; i++) _materials.push_back(materials[i]);
    }
    
    void setVertices(std::vector<Vertex> vertices) {
        size_t size = vertices.size();
        for (int i = 0; i < size; i++) _vertices.push_back(vertices[i]);
    }
    
    void setMeshes(std::vector<Mesh> meshes) {
        size_t size = meshes.size();
        for (int i = 0; i < size; i++) _meshes.push_back(meshes[i]);
    }
    
    void setSpheres(std::vector<Sphere> sphere) {
        size_t size = sphere.size();
        for (int i = 0; i < size; i++) _spheres.push_back(sphere[i]);
    }
    
    void setCameras(std::vector<Camera> camera) {
        size_t size = camera.size();
        for (int i = 0; i < size; i++) _cameras.push_back(camera[i]);
    }
    
    Material getMaterial(int key) {
        return _materials[key - 1];
    }
    
    Vertex& getVertex(int key) {
        return _vertices[key - 1];
    }
};

extern Scene* CurrentScene;

void ReadScene(int argc, char** argv);

#endif //RAYTRACER_SCENE_H
