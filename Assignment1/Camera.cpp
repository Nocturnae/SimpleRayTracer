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

Camera::Camera(int cameraID, Vector3 position, Vector3 gaze, Vector3 up, float left, float right, float bottom, float top, float distance, int horResolution, int verResolution, std::string outputFile) {
    _cameraID = cameraID;
    _position = position;
    _gaze = gaze;
    
    // _space
    _space.Forward = gaze;
    _space.Up = up;
    /*
    _space.Left._data[0] = (gaze._data[1] * up._data[2]) - (gaze._data[2] * up._data[1]);
    _space.Left._data[1] = (gaze._data[2] * up._data[0]) - (gaze._data[0] * up._data[2]);
    _space.Left._data[2] = (gaze._data[0] * up._data[1]) - (gaze._data[1] * up._data[0]);
     */
    _space.Left._data[0] = (up._data[1] * gaze._data[2]) - (up._data[2] * gaze._data[1]) * -1;
    _space.Left._data[1] = (up._data[2] * gaze._data[0]) - (up._data[0] * gaze._data[2]) * -1;
    _space.Left._data[2] = (up._data[0] * gaze._data[1]) - (up._data[1] * gaze._data[0]) * -1;
    
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
    // Vector3 cameraForward = _space.Forward
    Vector3 cameraUp = _space.Up, cameraLeft = _space.Left;
    
    Image outputImage(width, height, CurrentScene->Background());
    
    for (int h = 0; h < height; h++) {
       for (int w = 0; w < width; w++) {
            // compute viewing ray
           // slightly wrong. fix.
            float u = (left + ((right - left) * (w + 0.5) / width));
            float v = (bottom + ((top - bottom) * (h + 0.5) / height));
           
            Ray viewingRay(_position, (_gaze * distance) + (cameraLeft * u) + (cameraUp * v));
            int intersectionTime = INT_MAX;
            
            RayHitInfo rayHitInfo;
            for (const auto& sphere : CurrentScene->Spheres()) {
                if (sphere.Intersect(viewingRay, rayHitInfo)) {
                    Vector3 distanceVector = rayHitInfo.Position - _position;
                    int t = distanceVector.length();
                    if (t < intersectionTime) {
                        intersectionTime = t;
                    }
                }
            }
           
           for (const auto& mesh : CurrentScene->Meshes()) {
               for (const auto& triangle : mesh._triangles) {
                   if (triangle.Intersect(viewingRay, rayHitInfo)) {
                       Vector3 distanceVector = rayHitInfo.Position - _position;
                       int t = distanceVector.length();
                       if (t < intersectionTime) {
                           intersectionTime = t;
                       }
                   }
               }
           }
           
            if (intersectionTime < INT_MAX) {
                Color pixelColor = CurrentScene->Ambient();
                for (const auto& light : CurrentScene->Lights()) {
                    // TODO: compute shadow ray
                }
                outputImage.Pixel(w, h) = pixelColor;
            }
        }
    }
    
    return outputImage;
}
