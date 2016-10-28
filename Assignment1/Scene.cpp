//
//  Scene.cpp
//  Assignment1
//
//  Created by Zeynep Akkalyoncu on 27/10/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Scene.h"

Scene* CurrentScene;

void ReadScene(int argc, char** argv)
{
    /*
     * Reads the scene and cameras from arguments
     * Then sets the CurrentScene pointer
     */
    
    std::ifstream scene_file, camera_file;
    
    scene_file.open(argv[1]);
    
    // SCENE CREATION
    
    CurrentScene = new Scene();
    
    if (scene_file.good()) {
        // ray reflection count
        int rayReflectNum;
        scene_file >> rayReflectNum;
        CurrentScene->setRayReflect(rayReflectNum);
        
        // background color
        float background_x, background_y, background_z;
        scene_file >> background_x >> background_y >> background_z;
        Color backgroundColor(background_x, background_y, background_z);
        CurrentScene->setBackground(backgroundColor);
        
        // ambient light
        float ambient_x, ambient_y, ambient_z;
        scene_file >> ambient_x >> ambient_y >> ambient_z;
        Color ambientLight(ambient_x, ambient_y, ambient_z);
        CurrentScene->setAmbient(ambientLight);
        
        // point light
        int pointLightCount;
        scene_file >> pointLightCount;
        
        std::vector<PointLight> pointLightList;
        for (int i = 0; i < pointLightCount; i++) {
            Vector3 points, intensity;
            scene_file >> points;
            scene_file >> intensity;
            PointLight pointLight(points, intensity);
            pointLightList.push_back(pointLight);
        }
        
        CurrentScene->setPointLights(pointLightList);
        
        // material
        int materialCount;
        scene_file >> materialCount;
        
        std::vector<Material> materialList;
        for (int i = 0; i < materialCount; i++) {
            std::string materialTag;
            scene_file >> materialTag;
            int materialID;
            scene_file >> materialID;
            Vector3 ambient, diffuse, rgb, reflectance;
            float phong;
            scene_file >> ambient >> diffuse >> rgb;
            scene_file >> phong;
            scene_file >> reflectance;
            Material material(materialID, ambient, diffuse, rgb, phong, reflectance);
            materialList.push_back(material);
        }
        
        CurrentScene->setMaterials(materialList);
        
        // vertices
        int vertexCount;
        scene_file >> vertexCount;
        
        std::string vertexTag;
        scene_file >> vertexTag; scene_file >> vertexTag;
        
        std::vector<Vertex> vertexList;
        for (int i = 0; i < vertexCount; i++) {
            Vector3 coordinates;
            scene_file >> coordinates;
            Vertex vertex(coordinates);
            vertexList.push_back(vertex);
        }
        
        CurrentScene->setVertices(vertexList);
        
        // models
        int modelCount;
        scene_file >> modelCount;
        
        std::vector<Mesh> meshes;
        std::vector<Sphere> spheres;
        
        for (int i = 0; i < modelCount; i++) {
            std::string tag;
            scene_file >> tag;
            
            if (tag[1] == 'M') { // mesh
                int meshID;
                scene_file >> meshID;
                
                int triangleCount;
                scene_file >> triangleCount;
                
                int materialID;
                scene_file >> materialID;
                // not sure if it works this way?
                Material material = CurrentScene->getMaterial(materialID);
                
                std::vector<Triangle> triangles;
                for (int i = 0; i < triangleCount; i++) {
                    int vid1, vid2, vid3;
                    scene_file >> vid1 >> vid2 >> vid3;
                    
                    Vertex v1 = CurrentScene->getVertex(vid1);
                    Vertex v2 = CurrentScene->getVertex(vid2);
                    Vertex v3 = CurrentScene->getVertex(vid3);
                    
                    Triangle triangle(v1, v2, v3);
                    triangles.push_back(triangle);
                }
                
                Mesh mesh(meshID, material, triangles);
                meshes.push_back(mesh);
            }
            else if (tag[1] == 'S') { // sphere
                int sphereID;
                scene_file >> sphereID;
                
                int materialID;
                scene_file >> materialID;
                // not sure if it works this way?
                Material material = CurrentScene->getMaterial(materialID);
                
                float radius;
                scene_file >> radius;
                
                int vid1;
                scene_file >> vid1;
                Vertex center = CurrentScene->getVertex(vid1);
                
                Sphere sphere(sphereID, material, radius, center);
                spheres.push_back(sphere);
            }
        }
        
        CurrentScene->setMeshes(meshes);
        CurrentScene->setSpheres(spheres);
        
    }
    
    scene_file.close();
    
    // CAMERA CONSTRUCTION
    
    camera_file.open(argv[2]);
    
    std::vector<Camera> cameras;
    if (camera_file.good()) {
        int cameraCount;
        camera_file >> cameraCount;
        
        for (int i = 0; i < cameraCount; i++) {
            int cid;
            std::string cameraTag;
            camera_file >> cameraTag;
            camera_file >> cid;
            
            Vector3 position, gaze, up;
            camera_file >> position >> gaze >> up;
            
            float left, right, bottom, top, distance;
            int horResolution, verResolution;
            camera_file >> left >> right >> bottom >> top >> distance >> horResolution >> verResolution;
            
            std::string outputFile;
            camera_file >> outputFile;
            
            Camera camera(cid, position, gaze, up, left, right, bottom, top, distance, horResolution, verResolution, outputFile);
            cameras.push_back(camera);
        }
    }
    
    CurrentScene->setCameras(cameras);

    camera_file.close();
}
