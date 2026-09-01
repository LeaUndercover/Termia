//
// Created by cutie on 28.08.26.
//

#include "Note.h"

#include <cmath>
#include <iostream>
#include <ostream>

int Note::QuantizePosition(double curTime, double scrollSpeedMS, int positions) const {
    const auto delta = _time - curTime;
    const auto factor = delta / scrollSpeedMS;

    return positions-std::round(factor*positions);
}

bool Note::IsTooLate(double curTime, double od) {

    double delta = _time-curTime;

    if (delta>0)
        return false;

    if (IsHit())
        return false;

    if (IsInHitWindow(delta,od))
        return false;

    return true;
}
