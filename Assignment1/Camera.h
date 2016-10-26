#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Vector3.h"
#include "Image.h"
//#include "Scene.h"
#include "Sphere.h"

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
    
    Camera(int cameraID, Vector3 position, Vector3 gaze, Vector3 up, float left, float right, float bottom, float top, float distance, int horResolution, int verResolution, std::string outputFile) {
        _cameraID = cameraID;
        _position = position;
        _gaze = gaze;
        
        // _space
        _space.Forward = gaze;
        _space.Up = up;
        _space.Left._data[0] = (gaze._data[1] * up._data[2]) - (gaze._data[2] * up._data[1]);
        _space.Left._data[1] = (gaze._data[2] * up._data[0]) - (gaze._data[0] * up._data[2]);
        _space.Left._data[2] = (gaze._data[0] * up._data[1]) - (gaze._data[1] * up._data[0]);
        
        // _imagePlane
        _imagePlane.Left = left; _imagePlane.Right = right; _imagePlane.Bottom = bottom; _imagePlane.Top = top;
        _imagePlane.Distance = distance;
        _imagePlane.Width = horResolution;
        _imagePlane.Height = verResolution;
        
        _outputFile = outputFile;
    }

    /*
     * BEWARE! You should render each camera by using this function, we will grade it according to regulations!
     */
    Image Render() const;

    const std::string& OutputFile() const
    {
        return _outputFile;
    }
};
/*
Image Camera::Render() const {
    
    for (int i = 0; i < _imagePlane.Width; i++) {
        for (int j = 0; j < _imagePlane.Height; j++) {
            int intersectionTime = INT_MIN;
            Sphere closestSphere = NULL; // set null somehow
            
            for (const auto& sphere : CurrentScene->Spheres()) {
                
            }
            
            
        }
    }
}
 */
#endif //RAYTRACER_CAMERA_H
