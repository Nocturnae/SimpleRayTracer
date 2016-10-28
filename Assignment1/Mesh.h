#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include "Vertex.h"
#include "Triangle.h"

class Mesh {
private:
    int _meshID;
    Material _material;
public:
    std::vector<Triangle> _triangles;
    Mesh(int meshID, Material material, std::vector<Triangle> triangles) : _material(material) {
        _meshID = meshID;
        //for (int i = 0; i < triangles.size(); i++) _triangles[i] = triangles[i];
        _triangles = triangles;
    }
};

#endif //RAYTRACER_MESH_H
