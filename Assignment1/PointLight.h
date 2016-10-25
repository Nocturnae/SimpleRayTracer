#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

class PointLight {
private:
    Vector3 _position;
    Vector3 _intensity;
public:
    // TODO
    PointLight(Vector3 position, Vector3 intensity) {
        _position = position;
        _intensity = intensity;
    }
};

#endif //RAYTRACER_POINTLIGHT_H
