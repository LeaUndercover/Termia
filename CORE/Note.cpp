#include "Note.h"
#include <cmath>
#include <iostream>

#include "Chart.h"
using namespace CORE;

int Note::QuantizePosition(double curTime, int positions) const
{
    const double startFactor = GetVisualDistance(_time, curTime);
    const int startPosition = positions - std::round(startFactor * positions);

    return startPosition;
}

bool Note::IsTooLate(double curTime, double od) {

    double delta = _time-curTime+_offset;

    if (delta>0)
        return false;

    if (IsHit())
        return false;

    if (IsInHitWindow(delta,od))
        return false;

    return true;
}
