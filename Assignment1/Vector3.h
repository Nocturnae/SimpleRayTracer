#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3
{
public:
    float _data[3];

    friend std::istream &operator>>(std::istream &stream, Vector3 &vertex)
    {
        return stream >> vertex._data[0] >> vertex._data[1] >> vertex._data[2];
    }
    
    Vector3() : _data{0, 0, 0} {} // risky
    Vector3(float d1, float d2, float d3) : _data{d1, d2, d3} {}
    
    Vector3(const Vector3& rhs) {
        _data[0] = rhs._data[0];
        _data[1] = rhs._data[1];
        _data[2] = rhs._data[2];
    }
    
    void operator=(const Vector3& rhs) {
        if (this != &rhs) {
            _data[0] = rhs._data[0];
            _data[1] = rhs._data[1];
            _data[2] = rhs._data[2];
        }
    }
    
    float& operator[](int i) {
        return _data[i];
    }
    
    friend Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1._data[0] - v2._data[0],
                            v1._data[1] - v2._data[1],
                            v1._data[2] - v2._data[2]);
    }
    
    friend Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1._data[0] + v2._data[0],
                       v1._data[1] + v2._data[1],
                       v1._data[2] + v2._data[2]);
    }
    
    friend Vector3 operator*(const Vector3& v1, float scalar) {
        return Vector3(v1._data[0] * scalar,
                       v1._data[1] * scalar,
                       v1._data[2] * scalar);
    }
    
    friend Vector3 operator*(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1._data[0] * v2._data[0],
                       v1._data[1] * v2._data[1],
                       v1._data[2] * v2._data[2]);
    }
    
    friend Vector3 operator/(const Vector3& v1, float scalar) {
        return Vector3(v1._data[0] / scalar,
                       v1._data[1] / scalar,
                       v1._data[2] / scalar);
    }
    
    float dotProduct(Vector3& rhs) {
        return ((_data[0] * rhs._data[0]) + (_data[1] * rhs._data[1]) + (_data[2] * rhs._data[2]));
    }
    
    float length() const {
        return sqrt(pow(_data[0], 2) + pow(_data[1], 2) + pow(_data[2], 2));
    }
};

#endif //MATH_VECTOR3_H
