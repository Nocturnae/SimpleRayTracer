//
//  Texture.h
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 18/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include "Vector3.h"
#include "Vertex.h"
#include "Color.h"
extern "C" {
#include "jpeg_reader/aoa_jpeg.h"
}

class Texture {
    int _width, _height;
    UCOLOR** _image;
public:
    Texture(const char* file) {
        /*
        read_jpeg_header(file, &_width, &_height);
        
        _image = new UCOLOR*[_height];
        for (int i = 0; i < _height; i++) {
            _image[i] = new UCOLOR[_width];
        }
        
        read_jpeg(file, _image, &_width, &_height);
         */
    }
    
    Color Interpolate(Vector3 point, Vertex center, float radius) const;
};

typedef size_t TextureId;

#endif
