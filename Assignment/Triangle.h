//
// Created by Fatih on 10/27/15.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Vertex.h"
#include "Ray.h"
#include "Material.h"
#include "Texture.h"

class Scene;

typedef size_t TriangleId;

class Triangle
{
    const Scene * _scene;
    
    TriangleId _triangleId;

    VertexId    _vertices[3];

    MaterialId  _materialId;
    TextureId _textureId;

    Vector3     _normal;
    Vector3     _unitNormal;

    void calculateNormals();

public:

    explicit Triangle(TriangleId triangleId, const VertexId (&vertices)[3], MaterialId materialId, TextureId textureId);

    void SetScene(const Scene* scene);

	const VertexId(&Vertices()const)[3]
	{
		return _vertices;
	}

    bool HitParameter(const Ray& ray, float& parameter) const;

    bool RayHit(const Ray& ray, float t, RayHitInfo& hitInfo) const;

    const Vector3& Normal(const Vector3& at) const
    {
        return _unitNormal;
    }
    
    Vector3* GetTextureTriangle() const;

};

#endif //RAYTRACER_TRIANGLE_H

