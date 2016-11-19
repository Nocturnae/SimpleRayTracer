//
//  Texture.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 18/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Texture.h"
#include <cmath>
#include <algorithm>

Color Texture::Interpolate(Vector3 pointOfIntersection, Vertex center, float radius) const {
    
    float theta = acosf((pointOfIntersection[1] - center.Position()[1]) / radius);
    float phi = atanf((pointOfIntersection[2] - center.Position()[2]) / pointOfIntersection[0]);
    //float phi = atan2f((pointOfIntersection[2] - center.Position()[2], pointOfIntersection[0] - center.Position()[0]);
    
    float u = phi / (2 * M_PI), v = (M_PI - theta) / M_PI;
    
    int i = std::max(0, (int)std::round(u * _width)), j = std::max(0, (int)std::round(v * _height));
    
    //std::cout << i << " " << j << std::endl;
    
    //return Color(_image[i][j][0], _image[i][j][1], _image[i][j][2]); // ?
    
    return Color(0, 0, 0);
}
