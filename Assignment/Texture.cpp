//
//  Texture.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 20/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Texture.h"
#include <cmath>

Color* Texture::InterpolateSphere(Vector3 pointOfIntersection, Vertex center, float radius) const {
    // TODO: fix distorted image
    /*float theta = acosf((pointOfIntersection[1] - center.Position()[1]) / radius);*/
    float phi = atanf((pointOfIntersection[2] - center.Position()[2]) / (pointOfIntersection[0] - center.Position()[0]));
    float theta = acosf((pointOfIntersection[1] - center.Position()[1]) / radius);
    //float phi = atan2f(pointOfIntersection[2] - center.Position()[2], pointOfIntersection[0] - center.Position()[0]);
    /*
    if (phi < 0) {
        phi += 2 * M_PI;
    }*/
    
    float u = phi / (2 * M_PI), v = (M_PI - theta) / M_PI;
    
    int i = std::max(0, (int)std::round(u * _width)), j = std::max(0, (int)std::round(v * _height));
    //int i = std::round(u * _width), j = std::round(v * _height);
    
    //std::cout << pointOfIntersection << " " << theta << " " << phi << " " << i << " " << j << " " << u << " " << v << std::endl;
    
    Color* res = new Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]);
    return res;
}

Color* Texture::InterpolateTriangle(Vector3 textureTriangle[3], float beta, float gamma) const {

    float u = textureTriangle[0][0] + (beta * (textureTriangle[1][0] - textureTriangle[0][0])) + (gamma * (textureTriangle[2][0] - textureTriangle[0][0]));
    float v = textureTriangle[0][1] + (beta * (textureTriangle[1][1] - textureTriangle[0][1])) + (gamma * (textureTriangle[2][1] - textureTriangle[0][1]));
    
    //int i = std::round(u * _width), j = std::round(v * _height);
    int i = std::round(u * _width), j = std::round(v * _height);
    
    Color* res = new Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]);
    //Color* res = new Color(3, 28, 212);
    return res;
}
