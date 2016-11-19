//
//  Matrix.h
//  RayTracer
//
//  Created by Zeynep Akkalyoncu on 19/11/16.
//  Copyright © 2016 Zeynep Akkalyoncu. All rights reserved.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "Vector3.h"

class Matrix {
    Vector3 _mtrx[4];
public:
    Matrix() : _mtrx{Vector3::Zero} {};
    
    Matrix(Vector3 r1, Vector3 r2, Vector3 r3, Vector3 r4) {
        _mtrx[0] = r1;
        _mtrx[1] = r2;
        _mtrx[2] = r3;
        _mtrx[3] = r4;
    }
    
    Vector3& operator[](int index) {
        return _mtrx[index];
    }
    
    Matrix operator*(Matrix rhs) const {
        Matrix res;
        
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
    
    Vector3 operator*(Vector3 rhs) const {
        Vector3 res(Vector3::Dot(_mtrx[0], rhs),
                    Vector3::Dot(_mtrx[1], rhs),
                    Vector3::Dot(_mtrx[2], rhs),
                    Vector3::Dot(_mtrx[3], rhs));
        
        return res;
    }
};

#endif
