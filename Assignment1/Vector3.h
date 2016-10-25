#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

#include <iostream>

class Vector3
{
public:
    float _data[3];

    friend std::istream &operator>>(std::istream &stream, Vector3 &vertex)
    {
        return stream >> vertex._data[0] >> vertex._data[1] >> vertex._data[2];
    }
    
    Vector3& operator=(const Vector3& rhs) {
        if (this == &rhs) {
            _data[0] = rhs._data[0];
            _data[1] = rhs._data[1];
            _data[2] = rhs._data[2];
        }
        return *this;
    }
};

#endif //MATH_VECTOR3_H
