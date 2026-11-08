#include "Note.h"
#include <cmath>
#include <iostream>

#include "Chart.h"
using namespace CORE;

double Note::GetPosition(const double curTime) const
{
    const double startFactor = GetVisualDistance(_time, curTime);

    return startFactor;
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
