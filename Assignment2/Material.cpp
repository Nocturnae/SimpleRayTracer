//
// Created by Fatih on 10/30/15.
//

#include "Material.h"
#include "Scene.h"
#include <algorithm>
#include <cmath>
#include "Ray.h"


Material Material::Diffuse(const Color &color)
{
    Material m;
    m._diffuse = color;
    return m;
}

Color Material::Calculate(const Vector3 &normal, const Vector3 &pointOfIntersection, const Vector3 &viewDirection,
                          const Scene *scene, int hitCount) const
{
    float _channels[3] =
            {
                    scene->AmbientLight().R() * _ambient.R(),
                    scene->AmbientLight().G() * _ambient.G(),
                    scene->AmbientLight().B() * _ambient.B()
            };

    if (hitCount && _reflectance.R() > 0 && _reflectance.G() > 0 && _reflectance.B() > 0) {
        Color reflectionColor;
        CalculateReflectance(reflectionColor, normal, viewDirection, pointOfIntersection, scene, hitCount);
        _channels[0] += reflectionColor.R() * _reflectance.R();
        _channels[1] += reflectionColor.G() * _reflectance.G();
        _channels[2] += reflectionColor.B() * _reflectance.B();
    }

    Ray shadowRay(pointOfIntersection + normal * 0.0001f, Vector3::Zero);
    for (const auto &light : scene->Lights()) {

        RayHitInfo shadowInfo;
        float distanceSqr;

        Vector3 lightRayDir = light.Position() - pointOfIntersection;
        distanceSqr = lightRayDir.SquaredMagnitude();
        lightRayDir.Normalize();

        shadowRay.Direction(lightRayDir);
        if (scene->Raycast<false>(shadowRay, shadowInfo) && shadowInfo.Parameter * shadowInfo.Parameter < distanceSqr) {
            continue;
        }

        Color intensity = light.Intensity(pointOfIntersection);
        Vector3 lightViewHalf = (viewDirection + lightRayDir).Normalized();

        float diffuseCoefficient = std::max(0.0f, Vector3::Dot(normal, lightRayDir));
        float specularCoefficient = std::pow(std::max(0.0f, Vector3::Dot(normal, lightViewHalf)), _phong);

        _channels[0] += intensity.R() * (_diffuse.R() * diffuseCoefficient + _specular.R() * specularCoefficient);

        _channels[1] += intensity.G() * (_diffuse.G() * diffuseCoefficient + _specular.G() * specularCoefficient);

        _channels[2] += intensity.B() * (_diffuse.B() * diffuseCoefficient + _specular.B() * specularCoefficient);
    }

    return Color(_channels[0], _channels[1], _channels[2]);
}

void Material::CalculateReflectance(Color &reflectedColor, const Vector3 &normal, const Vector3 &rayDirection,
                                    const Vector3 &pointOfIntersection, const Scene *scene, int hitCount) const
{
    float cosAlpha = Vector3::Dot(rayDirection, normal);
    Vector3 reflectedRayDirection = (2 * cosAlpha * normal - rayDirection).Normalized();
    Ray reflectedRay(pointOfIntersection + normal * 0.0001, reflectedRayDirection);

    RayHitInfo hitInfo;
    if (scene->FastRaycast(reflectedRay, hitInfo)) {
        reflectedColor =
                scene->GetMaterial(hitInfo.Material).Calculate(hitInfo.Normal, hitInfo.Position,
                                                               -reflectedRay.Direction(), scene, hitCount - 1);
    }
}

std::istream &operator>>(std::istream &stream, Material &mat)
{
    stream >> mat._ambient;
    stream >> mat._diffuse;
    stream >> mat._specular;
    stream >> mat._phong;
    stream >> mat._reflectance;

    return stream;
}

