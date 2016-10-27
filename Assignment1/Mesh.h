#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include "Vertex.h"
#include "Triangle.h"

class Mesh {
private:
    int _meshID;
    int _triangleCount;
    Material _material;
    Vertex _vertices[3]; // TODO: remove
    // TODO: add triangles
    std::vector<Triangle> _triangles;
public:
    Mesh(int meshID, int triangleCount, Material material, Vertex vertices[3]) : _material(material) {
        _meshID = meshID;
        _triangleCount = triangleCount;
        for (int i = 0; i < 3; i++) _vertices[i] = vertices[i];
    }
};

#endif //RAYTRACER_MESH_H
