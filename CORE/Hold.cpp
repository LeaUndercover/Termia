#include "Hold.h"
#include <cmath>

#include "Chart.h"
using namespace CORE;

std::pair<double, double> Hold::GetPosition(const double curTime) const
{
    const double startFactor = GetVisualDistance(_time, curTime);
    const double endFactor = GetVisualDistance(_time+_length, curTime);

    return { startFactor, endFactor };
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
