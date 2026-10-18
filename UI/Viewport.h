//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_VIEWPORT_H
#define TERMIA_VIEWPORT_H
#include "../nikrend/Geometry/Size2.h"

class Viewport {
private:
    Size2 _size;
public:
    Viewport(const Size2 size): _size(size) {};

    Size2 GetSize() const {
        return _size;
    };
};

#endif //TERMIA_VIEWPORT_H