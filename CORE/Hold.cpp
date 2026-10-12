#include "Hold.h"
#include <cmath>

#include "Chart.h"
using namespace CORE;

std::pair<int, int> Hold::QuantizePosition(const double curTime, const int positions) const
{
    const double startFactor = GetVisualDistance(_time, curTime);
    const int startPosition = positions - std::round(startFactor * positions);

    const double endFactor = GetVisualDistance(_time+_length, curTime);
    const int endPosition = positions - std::round(endFactor * positions);

    return { startPosition, endPosition };
}

double Hold::GetLength() const {
    return _length;
}

bool Hold::IsTooLate(double curTime, double od) {

    double delta = _time+_length-curTime+_offset;

    if (delta>0)
        return false;

    if (IsHit())
        return false;

    if (IsInHitWindow(delta,od))
        return false;

    return true;
}
