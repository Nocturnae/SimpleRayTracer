//
// Created by Fatih on 10/30/15.
//

#include "Mesh.h"

/*
Mesh::Mesh(size_t triCount, MaterialId matId, std::vector<Triangle> tris)
        : _triCount(triCount), _materialId(matId), _tris(tris)
{
}
*/

Mesh::Mesh(MaterialId matId, TextureId texId, std::vector<VertexId> vid)
: _triCount(12), _materialId(matId), _textureId(texId)
{
    _tris = {
        Triangle({vid[0], vid[1], vid[2]}, matId, texId),
        Triangle({vid[0], vid[2], vid[3]}, matId, texId),
        Triangle({vid[0], vid[3], vid[7]}, matId, texId),
        Triangle({vid[0], vid[7], vid[4]}, matId, texId),
        Triangle({vid[0], vid[4], vid[5]}, matId, texId),
        Triangle({vid[0], vid[5], vid[1]}, matId, texId),
        Triangle({vid[6], vid[5], vid[4]}, matId, texId),
        Triangle({vid[6], vid[4], vid[7]}, matId, texId),
        Triangle({vid[6], vid[7], vid[3]}, matId, texId),
        Triangle({vid[6], vid[3], vid[2]}, matId, texId),
        Triangle({vid[6], vid[2], vid[1]}, matId, texId),
        Triangle({vid[6], vid[1], vid[5]}, matId, texId)
    };
    
}

void Mesh::SetScene(const Scene *scene)
{
    _scene = scene;
    for (auto& tri : _tris) {
        tri.SetScene(scene);
    }
}
/*
std::istream &operator>>(std::istream &stream, Mesh &mesh)
{
    stream >> mesh._triCount;
    stream >> mesh._materialId;

    VertexId ids[3];
    mesh._tris.reserve(mesh._triCount);
    for (auto i = 0U; i < mesh._triCount; ++i) {
        stream >> ids[0] >> ids[1] >> ids[2];
        mesh._tris.push_back(Triangle(ids, mesh._materialId));
    }

    return stream;
}
*/
