//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_VECTOR2_H
#define TERMIA_VECTOR2_H
#include <vector>

class Vector2 {
private:

public:
    double x;
    double y;

    Vector2() : x(0), y(0) {} ;
    Vector2(const double x, const double y): x(x), y(y) {};

    static double GetSignedTriArea(const std::vector<Vector2>& triangles) {
        const auto p1 = triangles[0];
        const auto p2 = triangles[1];
        const auto p3 = triangles[2];
        
        return 0.5 * ((p2.y - p1.y) * (p2.x + p1.x) + (p3.y - p2.y) * (p3.x + p2.x) + (p1.y - p3.y) * (p1.x + p3.x));
    };

    static double EdgefuncP(const Vector2 p, const Vector2 v1, const Vector2 v2) {
        return (v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y) * (p.x - v1.x);
    }
};

#endif //TERMIA_VECTOR2_H