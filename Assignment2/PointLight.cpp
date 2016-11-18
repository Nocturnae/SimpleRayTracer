//
// Created by Fatih on 10/30/15.
//

#include "PointLight.h"

std::istream &operator>>(std::istream &stream, PointLight &light) {

    stream >> light._position;
    stream >> light._intensity;

    return stream;
}

