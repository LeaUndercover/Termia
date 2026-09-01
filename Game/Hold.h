//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_HOLD_H
#define TERMIA_HOLD_H
#include <utility>

#include "GameObject.h"


class Hold: public GameObject {
private:
    double _length;

public:
    Hold(const int lane, const double time, const int hitSound, const int type, const double length) : GameObject(lane,time,hitSound,type), _length(length) {};

    std::pair<int,int> QuantizePosition(double curTime, double scrollSpeedMS, int positions) const;
    double GetLength() const;

    bool IsTooLate(double curTime, double od) override;
};


#endif //TERMIA_HOLD_H