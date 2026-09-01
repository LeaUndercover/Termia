//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_NOTE_H
#define TERMIA_NOTE_H
#include "GameObject.h"


class Note: public GameObject {
public:
    Note(const int lane, const double time, const int hitSound, const int type) : GameObject(lane,time,hitSound,type) {};

    int QuantizePosition(double curTime, double scrollSpeedMS, int positions) const;
    bool IsTooLate(double curTime, double od) override;
};


#endif //TERMIA_NOTE_H