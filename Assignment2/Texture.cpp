//
//  Texture.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 18/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Texture.h"
#include <cmath>

Color Texture::Interpolate(Vector3 pointOfIntersection) const {
    /*
    float theta = acosf(pointOfIntersection[1]); // / radius?
    float phi = atanf(pointOfIntersection[2]);
    
    float u = phi / (2 * M_PI), v = (M_PI - theta) / M_PI;
    
    int i = std::round(u * _width), j = std::round(v * _height);
    
    return Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]); // ?*/
    return Color(0, 0, 0);
}
