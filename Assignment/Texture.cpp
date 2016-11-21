//
//  Texture.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 20/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Texture.h"
#include <cmath>

Color* Texture::InterpolateSphere(Vector3 pointOfIntersection, Vertex center, float radius, Matrix rotMatrix) const {
    
    Vector3 rotAxes = rotMatrix * (pointOfIntersection - center.Position());
    
    float theta = acosf(rotAxes[1] / radius);
    float phi = atan2f(rotAxes[2], rotAxes[0]);
    
    if (phi < 0.0001f) {
        phi += 2 * M_PI;
    }
    
    float u = (M_PI - phi) / (2 * M_PI), v = theta / M_PI;
    
    int j = std::max(0, std::min((int)std::round(u * _width), _width - 1)),
        i = std::max(0, std::min((int)std::round(v * _height), _height - 1));
    
    //std::cout << theta << " " << phi << " " << i << " " << j << " " << u << " " << v << std::endl;
    
    Color* res = new Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]);
    return res;
}

Color* Texture::InterpolateTriangle(Vector3 textureTriangle[3], float beta, float gamma) const {

    float u = textureTriangle[0][0] + (beta * (textureTriangle[1][0] - textureTriangle[0][0])) + (gamma * (textureTriangle[2][0] - textureTriangle[0][0]));
    float v = textureTriangle[0][1] + (beta * (textureTriangle[1][1] - textureTriangle[0][1])) + (gamma * (textureTriangle[2][1] - textureTriangle[0][1]));
    
    int j = std::max(0, std::min((int)std::round(u * _width), _width - 1)),
    i = std::max(0, std::min((int)std::round(v * _height), _height - 1));
    
    Color* res = new Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]);
    return res;
}
