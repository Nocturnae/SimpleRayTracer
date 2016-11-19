//
//  Texture.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 18/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Texture.h"
#include <cmath>

Color Texture::Interpolate(Vector3 pointOfIntersection, Vertex center, float radius) const {
    
    float theta = acosf(pointOfIntersection[2] - center.Position()[2] / radius);
    float phi = atan2f(pointOfIntersection[1] - center.Position()[1], pointOfIntersection[0] - center.Position()[0]);
    
    float u = phi / (2 * M_PI), v = (M_PI - theta) / M_PI;
    
    std::cout << theta << " " << phi << std::endl;
    
    int i = std::round(u * _width), j = std::round(v * _height);
    
    return Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]); // ?
    //return Color(0, 0, 0);
}
