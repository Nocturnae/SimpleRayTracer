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
#include <string>

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
        for (int i = 0; i < size; i++) meshes.push_back(meshes[i]);
    }
    
    void setSpheres(std::vector<Sphere> sphere) {
        size_t size = sphere.size();
        for (int i = 0; i < size; i++) sphere.push_back(sphere[i]);
    }
    
    void setCameras(std::vector<Camera> camera) {
        size_t size = camera.size();
        for (int i = 0; i < size; i++) camera.push_back(camera[i]);
    }
    
    Material getMaterial(int key) {
        return _materials[key - 1];
    }
    
    Vertex getVertex(int key) {
        return _vertices[key - 1];
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
    
    //CurrentScene = new Scene();
    
    // ray reflection count
    int rayReflectNum;
    std::cin >> rayReflectNum;
    CurrentScene->setRayReflect(rayReflectNum);
    
    // background color
    float background_x, background_y, background_z;
    std::cin >> background_x >> background_y >> background_z;
    Color backgroundColor(background_x, background_y, background_z);
    CurrentScene->setBackground(backgroundColor);
    
    // ambient light
    float ambient_x, ambient_y, ambient_z;
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
        int materialID;
        std::cin >> materialID;
        Vector3 ambient, diffuse, rgb, reflectance;
        float phong;
        std::cin >> ambient >> diffuse >> rgb;
        std::cin >> phong;
        std::cin >> reflectance;
        Material material(materialID, ambient, diffuse, rgb, phong, reflectance);
        materialList.push_back(material);
    }
    
    CurrentScene->setMaterials(materialList);
    
    // vertices
    int vertexCount;
    std::cin >> vertexCount;
    
    std::vector<Vertex> vertices;
    for (int i = 0; i < vertexCount; i++) {
        Vector3 coordinates;
        std::cin >> coordinates;
        Vertex vertex(coordinates);
        vertices.push_back(vertex);
    }
    
    CurrentScene->setVertices(vertices);
    
    // models
    int modelCount;
    std::cin >> modelCount;
    
    std::vector<Mesh> meshes;
    std::vector<Sphere> spheres;
    
    for (int i = 0; i < modelCount; i++) {
        std::string tag;
        std::cin >> tag;
        
        if (tag[1] == 'M') { // mesh
            int meshID;
            std::cin >> meshID;
            
            int triangleCount;
            std::cin >> triangleCount;
            
            int materialID;
            std::cin >> materialID;
            // not sure if it works this way?
            Material material = CurrentScene->getMaterial(materialID);
            
            int vid1, vid2, vid3;
            std::cin >> vid1 >> vid2 >> vid3;
            
            Vertex vertices[3];
            vertices[0] = CurrentScene->getVertex(vid1);
            vertices[1] = CurrentScene->getVertex(vid2);
            vertices[2] = CurrentScene->getVertex(vid3);
            
            Mesh mesh(meshID, triangleCount, material, vertices);
            meshes.push_back(mesh);
        }
        else if (tag[1] == 'S') { // sphere
            int sphereID;
            std::cin >> sphereID;
            
            int materialID;
            std::cin >> materialID;
            // not sure if it works this way?
            Material material = CurrentScene->getMaterial(materialID);
            
            float radius;
            std::cin >> radius;
            
            int vid1;
            std::cin >> vid1;
            Vertex center = CurrentScene->getVertex(vid1);
            
            Sphere sphere(sphereID, material, radius, center);
            spheres.push_back(sphere);
        }
    }
    
    CurrentScene->setMeshes(meshes);
    CurrentScene->setSpheres(spheres);
    
    // CAMERA CONSTRUCTION
    
    int cameraCount;
    std::cin >> cameraCount;
    
    std::vector<Camera> cameras;
    
    for (int i = 0; i < cameraCount; i++) {
        int cid;
        std::cin >> cid;
        
        Vector3 position, gaze, up;
        std::cin >> position >> gaze >> up;
        
        float left, right, bottom, top, distance;
        int horResolution, verResolution;
        std::cin >> left >> right >> bottom >> top >> distance >> horResolution >> verResolution;
        
        std::string outputFile;
        std::cin >> outputFile;
        
        Camera camera(cid, position, gaze, up, left, right, bottom, top, distance, horResolution, verResolution, outputFile);
        cameras.push_back(camera);
    }
    
    CurrentScene->setCameras(cameras);
    
    scene_file.close();
    camera_file.close();
}

#endif //RAYTRACER_SCENE_H
