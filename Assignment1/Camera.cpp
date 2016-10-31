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
    float distance = _imagePlane.Distance;
    Vector3 cameraUp = _space.Up, cameraLeft = _space.Left;
    
    Image outputImage(width, height, CurrentScene->Background());
    
    for (int h = 0; h < height; h++) {
       for (int w = 0; w < width; w++) {
            // compute viewing ray
           float u = (left + ((right - left) * (w + 0.5) / width));
           float v = (top - ((top - bottom) * (h + 0.5) / height));
           
           Vector3 rayDirection = (_gaze * distance) - (cameraLeft * u) + (cameraUp * v) ;
            rayDirection = rayDirection / rayDirection.length();
           
            Ray viewingRay(_position, rayDirection);
           
           float intersectionTime = __FLT_MAX__;
           
           RayHitInfo rayHitInfo;
           
           MaterialId rayHitMaterial = 0;
           Vector3 rayHitPosition, rayHitNormal;
           
           Color pointColor = CurrentScene->Background();
           
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
           
           if (intersectionTime < __FLT_MAX__) {
               Material objectMaterial = CurrentScene->getMaterial(rayHitMaterial);
               pointColor = objectMaterial.Calculate(CurrentScene, rayDirection, rayHitPosition, rayHitNormal, _position, 0);
           }
           
           outputImage.Pixel(h, w) = pointColor;
        }
    }
    
    return outputImage;
}
