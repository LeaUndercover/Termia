//
// Created by cutie on 11.10.26.
//

#ifndef TERMIA_TIMINGPOINT_H
#define TERMIA_TIMINGPOINT_H


class TimingPoint {
private:
    int _time;
    double _beatLength;
    int _beatCount;
    int _sampleSet;
    int _sampleIndex;
    int _volume;
    bool _uninherited;
    int _effects;
public:
    TimingPoint(const int time,
                const double beatLength,
                const int beatCount,
                const int sampleSet,
                const int sampleIndex,
                const int volume,
                const bool uninherited,
                const int _effects):
                _time(time),
                _beatLength(beatLength),
                _beatCount(beatCount),
                _sampleSet(sampleSet),
                _sampleIndex(sampleIndex),
                _volume(volume),
                _uninherited(uninherited),
                _effects(_effects) {};

    int GetTime() const;
    double GetBeatLength() const;
    int GetBeatCount() const;
    int GetSampleSet() const;
    int GetSampleIndex() const;
    int GetVolume() const;
    bool GetUninherited() const;
    int GetEffects() const;

    double GetSVModifier() const;
};


#endif //TERMIA_TIMINGPOINT_H