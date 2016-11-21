//
// Created by Fatih on 10/30/15.
//

#include "Mesh.h"

Mesh::Mesh(MaterialId matId, TextureId texId, std::vector<VertexId> vid)
: _triCount(12), _materialId(matId), _textureId(texId)
{
    _tris = {
        Triangle(0, {vid[0], vid[1], vid[2]}, matId, texId),
        Triangle(1, {vid[0], vid[2], vid[3]}, matId, texId),
        Triangle(2, {vid[0], vid[3], vid[7]}, matId, texId),
        Triangle(3, {vid[0], vid[7], vid[4]}, matId, texId),
        Triangle(4, {vid[0], vid[4], vid[5]}, matId, texId),
        Triangle(5, {vid[0], vid[5], vid[1]}, matId, texId),
        Triangle(6, {vid[6], vid[5], vid[4]}, matId, texId),
        Triangle(7, {vid[6], vid[4], vid[7]}, matId, texId),
        Triangle(8, {vid[6], vid[7], vid[3]}, matId, texId),
        Triangle(9, {vid[6], vid[3], vid[2]}, matId, texId),
        Triangle(10, {vid[6], vid[2], vid[1]}, matId, texId),
        Triangle(11, {vid[6], vid[1], vid[5]}, matId, texId)
    };
    
}

void Mesh::SetScene(const Scene *scene)
{
    _scene = scene;
    for (auto& tri : _tris) {
        tri.SetScene(scene);
    }
}
