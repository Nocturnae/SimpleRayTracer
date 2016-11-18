//
// Created by Fatih on 10/27/15.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Vertex.h"
#include "Ray.h"
#include "Material.h"

class Scene;

class Triangle
{
    const Scene * _scene;

    VertexId    _vertices[3];

    MaterialId  _materialId;

    Vector3     _normal;
    Vector3     _unitNormal;

    void calculateNormals();

public:

    explicit Triangle(const VertexId (&vertices)[3], MaterialId materialId);

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

};

#endif //RAYTRACER_TRIANGLE_H

