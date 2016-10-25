#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Color.h"
#include "PointLight.h"
#include "Vertex.h"

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
};

Scene* CurrentScene;

void ReadScene(int argc, char** argv)
{
    /*
     * Reads the scene and cameras from arguments
     * Then sets the CurrentScene pointer
     */
}

#endif //RAYTRACER_SCENE_H
