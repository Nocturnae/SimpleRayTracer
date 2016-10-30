#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Vector3.h"
#include "Ray.h"
#include "Image.h"

class Camera {
    int _cameraID;
    struct {
        Vector3 Forward, Up, Left;
    } _space;

    struct {
        float Left, Right, Bottom, Top;
        float Distance;

        int Width;
        int Height;

    } _imagePlane;

    Vector3 _position;
    Vector3 _gaze;

    std::string _outputFile;

public:
    
    Camera(int cameraID, Vector3 position, Vector3 gaze, Vector3 up, float left, float right, float bottom, float top, float distance, int horResolution, int verResolution, std::string outputFile);
    /*
     * BEWARE! You should render each camera by using this function, we will grade it according to regulations!
     */
    Image Render() const;

    const std::string& OutputFile() const
    {
        return _outputFile;
    }
    
    Color ColorPoint(Ray& viewingRay) const;
};

#endif //RAYTRACER_CAMERA_H
