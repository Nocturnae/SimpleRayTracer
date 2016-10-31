//
//  Material.cpp
//  Assignment1
//
//  Created by Zeynep Akkalyoncu on 30/10/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Material.h"
#include "Scene.h"

Color Material::Calculate(Scene* CurrentScene, Vector3 viewDirection, Vector3 rayHitPosition, Vector3 rayHitNormal, Vector3 cameraPosition, int depth) {
    
    if (depth > CurrentScene->Reflectance()) return CurrentScene->Background();
    
    // ambient
    Color pointColor(CurrentScene->Ambient().R() * _ambient[0],
                       CurrentScene->Ambient().G() * _ambient[1],
                       CurrentScene->Ambient().B() * _ambient[2]);
    
    Vector3 normalDirection = rayHitNormal;
    
    for (const auto& light : CurrentScene->Lights()) {
        
        Vector3 lightDirection = light.Position() - rayHitPosition;
        float lightDistance = lightDirection.length();
        lightDirection = lightDirection / lightDirection.length();
        
        // shadow
        Ray shadowRay(rayHitPosition, lightDirection);
        RayHitInfo shadowHitInfo;
        
        bool inShadow = false;
        
        for (const auto& mesh : CurrentScene->Meshes()) {
            if (inShadow) break;
            for (const auto& triangle : mesh._triangles) {
                if (triangle.Intersect(shadowRay, shadowHitInfo)) {
                    if ((shadowHitInfo.Parameter > 0.001) && (shadowHitInfo.Parameter < lightDistance)) {
                        inShadow = true;
                        break;
                    }
                }
            }
        }
        
        for (const auto& sphere : CurrentScene->Spheres()) {
            if (sphere.Intersect(shadowRay, shadowHitInfo)) {
                if ((shadowHitInfo.Parameter > 0.001) && (shadowHitInfo.Parameter < lightDistance)) {
                    inShadow = true;
                    break;
                }
            }
        }
        
        if (inShadow) continue;
        
        float nlDot = normalDirection.dotProduct(lightDirection);
        if (nlDot < 0) nlDot = 0;
        
        Vector3 visionDirection = cameraPosition - rayHitPosition;
        visionDirection = visionDirection / visionDirection.length();
        Vector3 visionLight = visionDirection + lightDirection;
        Vector3 halfDirection = visionLight / visionLight.length();
        float nhDot = normalDirection.dotProduct(halfDirection);
        if (nhDot < 0) nhDot = 0;
        
        Vector3 actualIntensity = light.Intensity() / (4 * M_PI * pow(lightDistance, 2));
        
        // diffuse
        Vector3 diffuseVector = _diffuse * actualIntensity * nlDot;
        if (diffuseVector._data[0] > 255) diffuseVector._data[0] = 255;
        if (diffuseVector._data[1] > 255) diffuseVector._data[1] = 255;
        if (diffuseVector._data[2] > 255) diffuseVector._data[2] = 255;
        Color diffuseColor(diffuseVector[0], diffuseVector[1], diffuseVector[2]);
        
        pointColor = pointColor + diffuseColor;
        
        // blinn-phong
        Vector3 bpVector = _specular.rgb * actualIntensity * pow(nhDot, _specular.phong);
        if (bpVector._data[0] > 255) bpVector._data[0] = 255;
        if (bpVector._data[1] > 255) bpVector._data[1] = 255;
        if (bpVector._data[2] > 255) bpVector._data[2] = 255;
        Color bpColor(bpVector[0], bpVector[1], bpVector[2]);
        
        pointColor = pointColor + bpColor;
    
        
        //pointColor = pointColor + diffuseColor + bpColor;
        if (pointColor.R() > 255) std::cout << "r" << std::endl;
         if (pointColor.G() > 255) std::cout << "g" << std::endl;
         if (pointColor.B() > 255) std::cout << "b" << std::endl;
    }
    
    // reflective
    if (_reflectance.length() != 0) {
        Vector3 reflectDirection = viewDirection - (rayHitNormal * viewDirection.dotProduct(rayHitNormal) * 2);
        reflectDirection = reflectDirection / reflectDirection.length();
        Ray reflectRay(rayHitPosition, reflectDirection);
        RayHitInfo reflectHitInfo;
        
        float intersectionTime = __FLT_MAX__;
        
        MaterialId reflectHitMaterial = 0;
        Vector3 reflectHitPosition, reflectHitNormal;
        
        for (const auto& sphere : CurrentScene->Spheres()) {
            if (sphere.Intersect(reflectRay, reflectHitInfo)) {
                if ((reflectHitInfo.Parameter < intersectionTime) && (reflectHitInfo.Parameter > 0.001)) {
                    reflectHitMaterial = reflectHitInfo.Material;
                    reflectHitPosition = reflectHitInfo.Position;
                    reflectHitNormal = reflectHitInfo.Normal;
                    intersectionTime = reflectHitInfo.Parameter;
                }
            }
        }
        
        for (const auto& mesh : CurrentScene->Meshes()) {
            for (const auto& triangle : mesh._triangles) {
                if (triangle.Intersect(reflectRay, reflectHitInfo)) {
                    if ((reflectHitInfo.Parameter < intersectionTime) && (reflectHitInfo.Parameter > 0.001)) {
                        reflectHitMaterial = reflectHitInfo.Material;
                        reflectHitPosition = reflectHitInfo.Position;
                        reflectHitNormal = reflectHitInfo.Normal;
                        intersectionTime = reflectHitInfo.Parameter;
                    }
                }
            }
        }
        
        if (intersectionTime < __FLT_MAX__) {
            Material objectMaterial = CurrentScene->getMaterial(reflectHitMaterial);
            Color reflectiveColor = objectMaterial.Calculate(CurrentScene, reflectDirection, reflectHitPosition, reflectHitNormal, rayHitPosition, depth + 1);
            pointColor = pointColor + Color(_reflectance[0] * reflectiveColor.R(),
                                            _reflectance[1] * reflectiveColor.G(),
                                            _reflectance[2] * reflectiveColor.B());
        }
    }
    
    return pointColor;
}
