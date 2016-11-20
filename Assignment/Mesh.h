//
// Created by Fatih on 10/30/15.
//

#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H


#include <iosfwd>
#include <vector>
#include "Material.h"
#include "Texture.h"
#include "Vertex.h"
#include "Triangle.h"

class Mesh {
    const Scene* _scene;

    size_t _triCount;
    MaterialId _materialId;
    TextureId _textureId;
    std::vector<Triangle> _tris;

public:

    Mesh() = default;
    //Mesh(size_t triCount, MaterialId matId, std::vector<Triangle> tris);
    Mesh(MaterialId matId, TextureId texId, std::vector<VertexId> vid);

	const std::vector<Triangle>& GetTris() const
	{
		return _tris;
	}

    //friend std::istream& operator>>(std::istream& stream, Mesh& mesh);

    void SetScene(const Scene* scene);
};


#endif //RAYTRACER_MESH_H
