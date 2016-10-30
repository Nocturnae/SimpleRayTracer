//
//  Material.cpp
//  Assignment1
//
//  Created by Zeynep Akkalyoncu on 30/10/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Material.h"
#include "Scene.h"

Color Material::Calculate(Scene* CurrentScene, Vector3 rayHitPosition, Vector3 rayHitNormal, Vector3 cameraPosition) {
    // ambient
    Color pointColor(CurrentScene->Ambient().R() * _ambient[0],
                       CurrentScene->Ambient().G() * _ambient[1],
                       CurrentScene->Ambient().B() * _ambient[2]);
    
    for (const auto& light : CurrentScene->Lights()) {
        
        Vector3 normalDirection = rayHitNormal;
        Vector3 lightDirection = light.Position() - rayHitPosition;
        float lightDistance = lightDirection.length();
        lightDirection = lightDirection / lightDirection.length();
        
        // intersect lightDirection with all objects to check if in shadow
        Ray shadowRay(rayHitPosition, lightDirection);
        RayHitInfo shadowHitInfo;
        
        bool inShadow = false;
        
        for (const auto& mesh : CurrentScene->Meshes()) {
            if (inShadow) break;
            for (const auto& triangle : mesh._triangles) {
                if (triangle.Intersect(shadowRay, shadowHitInfo)) {
                    if (shadowHitInfo.Parameter > 0.0001) {
                        inShadow = true;
                        break;
                    }
                }
            }
        }
        
        for (const auto& sphere : CurrentScene->Spheres()) {
            if (sphere.Intersect(shadowRay, shadowHitInfo)) {
                if (shadowHitInfo.Parameter > 0.0001) {
                    inShadow = true;
                    break;
                }
            }
        }
        
        if (inShadow) continue;
        
        float nlDot = normalDirection.dotProduct(lightDirection);
        if (nlDot < 0) nlDot = 0;
        
        Vector3 visionDirection = cameraPosition - rayHitPosition;
        Vector3 visionLight = visionDirection + lightDirection;
        Vector3 halfDirection = visionLight / visionLight.length();
        float nhDot = normalDirection.dotProduct(halfDirection);
        if (nhDot <= 0) nhDot = 0;
        
        Vector3 actualIntensity = light.Intensity() / (4 * M_PI * lightDistance * lightDistance);
        /*
         Vector3 rayOrigin = viewingRay.Origin(), rayDirection = viewingRay.Direction();
         Vector3 reflectedDirection = rayDirection - (rayHitNormal * 2 * rayDirection.dotProduct(rayHitNormal));
         reflectedDirection = reflectedDirection / reflectedDirection.length();
         Ray reflectedRay(rayHitPosition, reflectedDirection);
     */
        
        // diffuse
        Vector3 diffuseVector = _diffuse * actualIntensity * nlDot;
        if (diffuseVector._data[0] >= 255) diffuseVector._data[0] = 255;
        if (diffuseVector._data[1] >= 255) diffuseVector._data[1] = 255;
        if (diffuseVector._data[2] >= 255) diffuseVector._data[2] = 255;
        Color diffuseColor(diffuseVector[0], diffuseVector[1], diffuseVector[2]);
        
        // blinn-phong
        // check if exceeds 255?
        Vector3 bpVector = _specular.rgb * actualIntensity * pow(nhDot, _specular.phong);
        Color bpColor(bpVector[0], bpVector[1], bpVector[2]);
        
        pointColor = pointColor + diffuseColor + bpColor;
        
    }
    
    return pointColor;
}
