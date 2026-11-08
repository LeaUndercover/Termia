//
// Created by cutie on 19.10.26.
//

#include "Vector3.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "nikrend/nikrend_datatypes.h"

void Vector3::Rotate(const char axis, const double angle, const Vector3 &rotp) {
    // coordtransform center to rotp
    auto rotatedVector = Vector3(x,y,z);
    rotatedVector.x -= rotp.x;
    rotatedVector.y -= rotp.y;
    rotatedVector.z -= rotp.z;

    switch (axis) {
        case 'x': // zy rotation
            rotatedVector.z = z * cos(angle) - y * sin(angle);
            rotatedVector.y = z * sin(angle) + y * cos(angle);
            break;
        case 'y': // xz rotation
            rotatedVector.x = x * cos(angle) - z * sin(angle);
            rotatedVector.z = x * sin(angle) + z * cos(angle);
            break;
        case 'z': // xy rotation
            rotatedVector.x = x * cos(angle) - y * sin(angle);
            rotatedVector.y = x * sin(angle) + y * cos(angle);
            break;
        default:
            throw std::invalid_argument("axis");
    }

    rotatedVector.x += rotp.x;
    rotatedVector.y += rotp.y;
    rotatedVector.z += rotp.z;

    x=rotatedVector.x;
    y=rotatedVector.y;
    z=rotatedVector.z;
}

void Vector3::Rotate(const Vector3 &position, const Vector3 &rotation) {
    Rotate('x', rotation.x, position);
    Rotate('y', rotation.y, position);
    Rotate('z', rotation.z, position);
}

Vector2 Vector3::ProjectVector(const Vector3 &cameraPosition, const Vector3 &cameraRotation) const {
    auto copy = Vector3(x, y, z);

    copy.Rotate(cameraPosition, cameraRotation);

    const Vector2 returnVec = {
        (copy.x + cameraPosition.x) / copy.z,
        (copy.y + cameraPosition.y) / copy.z,
    };
    return returnVec;
}
