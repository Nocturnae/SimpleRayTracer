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
    _space.Left._data[0] = (gaze._data[1] * up._data[2]) - (gaze._data[2] * up._data[1]);
    _space.Left._data[1] = (gaze._data[2] * up._data[0]) - (gaze._data[0] * up._data[2]);
    _space.Left._data[2] = (gaze._data[0] * up._data[1]) - (gaze._data[1] * up._data[0]);
    
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
    Vector3 cameraForward = _space.Forward, cameraUp = _space.Up, cameraLeft = _space.Left;
    
    Image outputImage(width, height, CurrentScene->Background());
    /*
    Color* pixels = new Color[width * height];
    
    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            // is that the correct i/j?
            // compute viewing ray
            float u = left + ((right - left) * (w + 0.5) / width);
            float v = bottom + ((top - bottom) * (h + 0.5) / height);
            
            Ray viewingRay(_position, (_gaze * (-distance)) + (cameraLeft * u) + (cameraUp * v));
            int intersectionTime = INT_MIN;
            
            //Sphere closestSphere = NULL; // set null somehow
            
            for (const auto& sphere : CurrentScene->Spheres()) {
                RayHitInfo rayHitInfo;
                if (sphere.Intersect(viewingRay, rayHitInfo)) {
                    Vector3 distanceVector = rayHitInfo.Position - _position;
                    int t = distanceVector.length();
                    if (t < intersectionTime) {
                        intersectionTime = t;
                    }
                }
            }
            
            Color& pixel = outputImage.Pixel(w * width, h);
            if (intersectionTime > INT_MIN) {
                Color pixelColor = CurrentScene->Ambient();
                for (const auto& light : CurrentScene->Lights()) {
                    // TODO: compute shadow ray
                }
                pixel = pixelColor;
            }
        }
    }*/
    
    return outputImage;
}
