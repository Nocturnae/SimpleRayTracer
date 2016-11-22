//
//  Matrix.hpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 20/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "Vector3.h"

class Matrix {
    Vector3 _mtrx[4];
public:
    Matrix();
    
    Matrix(Vector3 r1, Vector3 r2, Vector3 r3, Vector3 r4);
    
    Vector3& operator[](int index);
    
    Matrix operator*(Matrix rhs) const;
    
    Vector3 operator*(Vector3 rhs) const;
};

#endif /* Matrix_hpp */
