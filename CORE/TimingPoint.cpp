//
// Created by cutie on 11.10.26.
//

#include "TimingPoint.h"

int TimingPoint::GetTime() const {
    return _time;
}

double TimingPoint::GetBeatLength() const {
    return _beatLength;
}

int TimingPoint::GetBeatCount() const {
    return _beatCount;
}

int TimingPoint::GetSampleSet() const {
    return _sampleSet;
}

int TimingPoint::GetSampleIndex() const {
    return _sampleIndex;
}

int TimingPoint::GetVolume() const {
    return _volume;
}

bool TimingPoint::GetUninherited() const {
    return _uninherited;
}

int TimingPoint::GetEffects() const {
    return _effects;
}

double TimingPoint::GetSVModifier() const {
    if (_uninherited)
        return 1.0;

    return -100.0 / _beatLength;
}





