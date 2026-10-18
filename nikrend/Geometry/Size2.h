//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_SIZE2_H
#define TERMIA_SIZE2_H

class Size2 {
private:
    float _width;
    float _height;
public:
    Size2(): _width(0.0f), _height(0.0f) {};
    Size2(const float w, const float h): _width(w), _height(h) {};

    float getWidth() const {
        return _width;
    };

    float getHeight() const {
        return _height;
    };
};

#endif //TERMIA_SIZE2_H