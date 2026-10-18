//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_VECTOR3_H
#define TERMIA_VECTOR3_H

class Vector3 {
private:
    float _x;
    float _y;
    float _z;

public:
    Vector3();
    Vector3(const float x, const float y, const float z): _x(x), _y(y), _z(z) {};
};

#endif //TERMIA_VECTOR3_H