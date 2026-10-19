//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_VECTOR3_H
#define TERMIA_VECTOR3_H
#include "Vector2.h"

class Vector3 {
private:

public:
    double x;
    double y;
    double z;
    
    Vector3();
    Vector3(const double x, const double y, const double z): x(x), y(y), z(z) {};

    Vector3 Rotate(char axis, double angle, const Vector3 &rotp) const;
    Vector2 ProjectVector(const Vector3 &cameraPosition, const Vector3 &cameraRotation) const;

    void Rotate(char axis, double angle, const Vector3 &rotp);
    void Rotate(const Vector3 &position, const Vector3 &rotation);
};

#endif //TERMIA_VECTOR3_H
