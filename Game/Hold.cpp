//
// Created by cutie on 29.08.26.
//

#include "Hold.h"

#include <cmath>

std::pair<int,int> Hold::QuantizePosition(double curTime, double scrollSpeedMS, int positions) const {
    const auto deltaStart = _time - curTime;
    const auto deltaEnd = (_time+_length) - curTime;
    const auto factorStart = deltaStart / scrollSpeedMS;
    const auto factorEnd = deltaEnd / scrollSpeedMS;

    int startPosition = positions-std::round(factorStart*positions);
    int endPosition = positions-std::round(factorEnd*positions);

    if (endPosition < 0 )
        endPosition = 0;

    if (startPosition > positions )
        startPosition = positions;

    std::pair<int,int> pair = {startPosition, endPosition};

    return pair;
}

double Hold::GetLength() const {
    return _length;
}

bool Hold::IsTooLate(double curTime, double od) {

    double delta = _time+_length-curTime;

    if (delta>0)
        return false;

    if (IsHit())
        return false;

    if (IsInHitWindow(delta,od))
        return false;

    return true;
}
