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
    /*_space.Forward = gaze / gaze.length();
    _space.Up = up /  up.length();*/
    _space.Forward = gaze;
    _space.Up = up;
    _space.Left._data[0] = (up._data[1] * gaze._data[2]) - (up._data[2] * gaze._data[1]);
    _space.Left._data[1] = (up._data[2] * gaze._data[0]) - (up._data[0] * gaze._data[2]);
    _space.Left._data[2] = (up._data[0] * gaze._data[1]) - (up._data[1] * gaze._data[0]);
    //_space.Left = _space.Left / _space.Left.length();
    
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
            float v = (bottom + ((top - bottom) * (h + 0.5) / height));
           
           Vector3 rayDirection = _position + (_gaze * distance) + (cameraLeft * u) + (cameraUp * v);
           rayDirection = rayDirection / rayDirection.length();
           
            Ray viewingRay(_position, rayDirection);
            float intersectionTime = __FLT_MAX__;
            
            RayHitInfo rayHitInfo;
            for (const auto& sphere : CurrentScene->Spheres()) {
                if (sphere.Intersect(viewingRay, rayHitInfo)) {
                    Vector3 distanceVector = rayHitInfo.Position - _position;
                    float t = distanceVector.length();
                    if (t < intersectionTime) {
                        intersectionTime = t;
                    }
                }
            }
           
           for (const auto& mesh : CurrentScene->Meshes()) {
               for (const auto& triangle : mesh._triangles) {
                   if (triangle.Intersect(viewingRay, rayHitInfo)) {
                       Vector3 distanceVector = rayHitInfo.Position - _position;
                       float t = distanceVector.length();
                       if (t < intersectionTime) {
                           intersectionTime = t;
                       }
                   }
               }
           }
           
           float diffuse_x = 0, diffuse_y = 0, diffuse_z = 0;
           if (intersectionTime < INT_MAX) {
               Color ambientColor = CurrentScene->Ambient();
               /*
               float diffuse_x = 0, diffuse_y = 0, diffuse_z = 0;
               for (const auto& light : CurrentScene->Lights()) {
                   // compute shadow ray
                   Ray reverseIntersectRay(rayHitInfo.Position, light.Position() - rayHitInfo.Position);
                   
                   Vector3 actualIntensity = light.Intensity() / (4 * M_PI * pow(reverseIntersectRay.Direction().length(), 2));
                   float nl = (rayHitInfo.Normal._data[0] * reverseIntersectRay.Direction()._data[0])
                                + (rayHitInfo.Normal._data[1] * reverseIntersectRay.Direction()._data[1])
                                    + (rayHitInfo.Normal._data[2] * reverseIntersectRay.Direction()._data[2]);
                   diffuse_x += actualIntensity._data[0] * nl;
                   diffuse_y += actualIntensity._data[1] * nl;
                   diffuse_z += actualIntensity._data[2] * nl;
               }
               */
               Color diffuseColor(diffuse_x, diffuse_y, diffuse_z);
               outputImage.Pixel(h, w) = ambientColor + diffuseColor;
            }
        }
    }
    
    return outputImage;
}
