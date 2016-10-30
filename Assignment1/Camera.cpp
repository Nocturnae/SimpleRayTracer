//
//  Camera.cpp
//  Assignment1
//
//  Created by Zeynep Akkalyoncu on 26/10/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include <stdio.h>
#include "Scene.h"
#include "Camera.h"
#include <cmath>

Camera::Camera(int cameraID, Vector3 position, Vector3 gaze, Vector3 up, float left, float right, float bottom, float top, float distance, int horResolution, int verResolution, std::string outputFile) {
    _cameraID = cameraID;
    _position = position;
    _gaze = gaze;
    // _space
    _space.Forward = gaze / gaze.length();
    _space.Up = up /  up.length();
    /*_space.Forward = gaze;
    _space.Up = up;*/
    _space.Left._data[0] = (up._data[1] * gaze._data[2]) - (up._data[2] * gaze._data[1]);
    _space.Left._data[1] = (up._data[2] * gaze._data[0]) - (up._data[0] * gaze._data[2]);
    _space.Left._data[2] = (up._data[0] * gaze._data[1]) - (up._data[1] * gaze._data[0]);
    _space.Left = _space.Left / _space.Left.length();
    
    // _imagePlane
    _imagePlane.Left = left; _imagePlane.Right = right; _imagePlane.Bottom = bottom; _imagePlane.Top = top;
    _imagePlane.Distance = distance;
    _imagePlane.Width = horResolution;
    _imagePlane.Height = verResolution;
    
    _outputFile = outputFile;
}

Image Camera::Render() const {
    
    int width = _imagePlane.Width, height = _imagePlane.Height;
    float left = _imagePlane.Left, right = _imagePlane.Right, bottom = _imagePlane.Bottom, top = _imagePlane.Top;
    float distance = _imagePlane.Distance; // change back; probably subtracting an unnecesary vector somewhere
    Vector3 cameraUp = _space.Up, cameraLeft = _space.Left;
    
    Image outputImage(width, height, CurrentScene->Background());
    
    for (int h = 0; h < height; h++) {
       for (int w = 0; w < width; w++) {
            // compute viewing ray
           // might be wrong
           
           /* float u = (left + ((right - left) * (w + 0.5) / width));
            float v = (bottom + ((top - bottom) * (h + 0.5) / height));*/
           float u = (left + ((right - left) * (w + 0.5) / width));
           float v = (top - ((top - bottom) * (h + 0.5) / height));
           
           //Vector3 rayDirection = (_gaze * distance) - (cameraUp * u) + (cameraLeft * v);
           Vector3 rayDirection = (_gaze * distance) - (cameraLeft * u) + (cameraUp * v) ;
           /*Vector3 rayDirection = _position + (_gaze * distance)
                                + (cameraLeft * left) + (cameraUp * top)
                                + (cameraLeft * u) - (cameraUp * v);*/
            rayDirection = rayDirection / rayDirection.length();
           
            Ray viewingRay(_position, rayDirection);
            float intersectionTime = __FLT_MAX__;
            
            RayHitInfo rayHitInfo;
           
           MaterialId rayHitMaterial = 0;
           Vector3 rayHitPosition, rayHitNormal;
           
            for (const auto& sphere : CurrentScene->Spheres()) {
                if (sphere.Intersect(viewingRay, rayHitInfo)) {
                    if (rayHitInfo.Parameter < intersectionTime) {
                        rayHitMaterial = rayHitInfo.Material;
                        rayHitPosition = rayHitInfo.Position;
                        rayHitNormal = rayHitInfo.Normal;
                        intersectionTime = rayHitInfo.Parameter;
                    }
                }
            }
           
           // should work either way, why doesn't it? something to do with intersection time
           for (const auto& mesh : CurrentScene->Meshes()) {
               for (const auto& triangle : mesh._triangles) {
                   if (triangle.Intersect(viewingRay, rayHitInfo)) {
                       if (rayHitInfo.Parameter < intersectionTime) {
                           rayHitMaterial = rayHitInfo.Material;
                           rayHitPosition = rayHitInfo.Position;
                           rayHitNormal = rayHitInfo.Normal;
                           intersectionTime = rayHitInfo.Parameter;
                       }
                   }
               }
           }
           /*
           for (const auto& sphere : CurrentScene->Spheres()) {
               if (sphere.Intersect(viewingRay, rayHitInfo)) {
                   if (rayHitInfo.Parameter < intersectionTime) intersectionTime = rayHitInfo.Parameter;
               }
           }*/
           
           if (intersectionTime < __FLT_MAX__) {
               Material objectMaterial = CurrentScene->getMaterial(rayHitMaterial);
               
               Color pointColor(CurrentScene->Ambient().R() * objectMaterial.Ambient()._data[0],
                                CurrentScene->Ambient().G() * objectMaterial.Ambient()._data[1],
                                CurrentScene->Ambient().B() * objectMaterial.Ambient()._data[2]);
               
               for (const auto& light : CurrentScene->Lights()) {
                   
                   Vector3 normalDirection = rayHitNormal;
                   Vector3 lightDirection = light.Position() - rayHitPosition;
                   float lightDistance = lightDirection.length();
                   lightDirection = lightDirection / lightDirection.length();
                   float nlDot = normalDirection.dotProduct(lightDirection);
                   if (nlDot < 0) nlDot = 0;
                   
                   Vector3 visionDirection = _position - rayHitPosition;
                   Vector3 visionLight = visionDirection + lightDirection;
                   Vector3 halfDirection = visionLight / visionLight.length();
                   float nhDot = normalDirection.dotProduct(halfDirection);
                   if (nhDot <= 0) nhDot = 0;
                   
                   //Vector3 actualIntensity = light.Intensity() / (4 * M_PI * pow(lightDirection.length(), 2));
                   Vector3 actualIntensity = light.Intensity() / (4 * M_PI * lightDistance * lightDistance);
                   
                   pointColor = pointColor + objectMaterial.Calculate(actualIntensity, nlDot, nhDot);
                   
               }
               
               outputImage.Pixel(h, w) = pointColor;
            }
        }
    }
    
    return outputImage;
}
