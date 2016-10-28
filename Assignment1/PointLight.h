#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

class PointLight {
private:
    Vector3 _position;
    Vector3 _intensity;
public:
    PointLight(Vector3 position, Vector3 intensity) {
        _position = position;
        _intensity = intensity;
    }
    
    Vector3 Intensity() const {
        return _intensity;
    }
};

#endif //RAYTRACER_POINTLIGHT_H
