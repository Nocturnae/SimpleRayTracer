//
//  Matrix.cpp
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 20/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#include "Matrix.h"

// TODO: destructors

Matrix::Matrix() {
    _mtrx[0] = Vector3(1, 0, 0, 0);
    _mtrx[1] = Vector3(0, 1, 0, 0);
    _mtrx[2] = Vector3(0, 0, 1, 0);
    _mtrx[3] = Vector3(0, 0, 0, 1);
}

Matrix::Matrix(Vector3 r1, Vector3 r2, Vector3 r3, Vector3 r4) {
    _mtrx[0] = r1;
    _mtrx[1] = r2;
    _mtrx[2] = r3;
    _mtrx[3] = r4;
}

Vector3& Matrix::operator[](int index) {
    return _mtrx[index];
}

Matrix Matrix::operator*(Matrix rhs) const {
    Matrix res;
    
    // TODO: check
    res[0][0] = (_mtrx[0][0] * rhs._mtrx[0][0]) + (_mtrx[0][1] * rhs._mtrx[1][0]) + (_mtrx[0][2] * rhs._mtrx[2][0]);
    res[0][1] = (_mtrx[0][0] * rhs._mtrx[0][1]) + (_mtrx[0][1] * rhs._mtrx[1][1]) + (_mtrx[0][2] * rhs._mtrx[2][1]);
    res[0][2] = (_mtrx[0][0] * rhs._mtrx[0][2]) + (_mtrx[0][1] * rhs._mtrx[1][2]) + (_mtrx[0][2] * rhs._mtrx[2][2]);
    
    res[1][0] = (_mtrx[1][0] * rhs._mtrx[0][0]) + (_mtrx[1][1] * rhs._mtrx[1][0]) + (_mtrx[1][2] * rhs._mtrx[2][0]);
    res[1][1] = (_mtrx[1][0] * rhs._mtrx[0][1]) + (_mtrx[1][1] * rhs._mtrx[1][1]) + (_mtrx[1][2] * rhs._mtrx[2][1]);
    res[1][2] = (_mtrx[1][0] * rhs._mtrx[0][2]) + (_mtrx[1][1] * rhs._mtrx[1][2]) + (_mtrx[1][2] * rhs._mtrx[2][2]);
    
    res[2][0] = (_mtrx[2][0] * rhs._mtrx[0][0]) + (_mtrx[2][1] * rhs._mtrx[1][0]) + (_mtrx[2][2] * rhs._mtrx[2][0]);
    res[2][1] = (_mtrx[2][0] * rhs._mtrx[0][1]) + (_mtrx[2][1] * rhs._mtrx[1][1]) + (_mtrx[2][2] * rhs._mtrx[2][1]);
    res[2][2] = (_mtrx[2][0] * rhs._mtrx[0][2]) + (_mtrx[2][1] * rhs._mtrx[1][2]) + (_mtrx[2][2] * rhs._mtrx[2][2]);
    
    res[3][0] = (_mtrx[3][0] * rhs._mtrx[0][0]) + (_mtrx[3][1] * rhs._mtrx[1][0]) + (_mtrx[3][2] * rhs._mtrx[2][0]);
    res[3][1] = (_mtrx[3][0] * rhs._mtrx[0][1]) + (_mtrx[3][1] * rhs._mtrx[1][1]) + (_mtrx[3][2] * rhs._mtrx[2][1]);
    res[3][2] = (_mtrx[3][0] * rhs._mtrx[0][2]) + (_mtrx[3][1] * rhs._mtrx[1][2]) + (_mtrx[3][2] * rhs._mtrx[2][2]);
    
    return res;
}

Vector3 Matrix::operator*(Vector3 rhs) const {
    
    Vector3 res((_mtrx[0][0] * rhs[0]) + (_mtrx[0][1] * rhs[1]) + (_mtrx[0][2] * rhs[2]) + (_mtrx[0][3] * rhs[3]),
                (_mtrx[1][0] * rhs[0]) + (_mtrx[1][1] * rhs[1]) + (_mtrx[1][2] * rhs[2]) + (_mtrx[1][3] * rhs[3]),
                (_mtrx[2][0] * rhs[0]) + (_mtrx[2][1] * rhs[1]) + (_mtrx[2][2] * rhs[2]) + (_mtrx[2][3] * rhs[3]),
                (_mtrx[3][0] * rhs[0]) + (_mtrx[3][1] * rhs[1]) + (_mtrx[3][2] * rhs[2]) + (_mtrx[3][3] * rhs[3]));
    
    return res;
}
