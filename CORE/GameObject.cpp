#include "GameObject.h"

#include "Chart.h"
using namespace CORE;

double GameObject::GetVisualDistance(const double targetTime, const double time) const
{
    auto config = TermiaConfig::GetInstance();
    const double scrollSpeedMS = config->GetScrollSpeedMS();
    const double speed = config->GetSpeed();

    if (scrollSpeedMS <= 0.0)
        return 0.0;

    // Apply offset consistently to the target time.
    const double effectiveTarget = targetTime;

    if (effectiveTarget <= time)
        return 0.0;

    double currentTime = effectiveTarget;
    double visualDistance = 0.0;

    auto* svPoint = _chart->GetSVPoint(currentTime);
    double bpmModifier = _chart->GetBPM(currentTime)/_chart->GetAverageBPM();
    while (currentTime > time)
    {
        if (svPoint == nullptr)
        {
            // Default scrolling before the first SV point.
            visualDistance += (currentTime - time) * speed;
            break;
        }

        const double boundary = svPoint->GetTime();
        const double segmentStart = std::max(time, boundary);
        const double segmentDuration = currentTime - segmentStart;

        visualDistance += segmentDuration
                        * svPoint->GetSVModifier()
                        * speed
                        * bpmModifier;

        currentTime = segmentStart;

        if (currentTime <= time)
            break;

        svPoint = _chart->GetSVPoint(boundary - 1.0);
        bpmModifier = _chart->GetBPM(boundary - 1.0) / _chart->GetAverageBPM();
    }

    return visualDistance / scrollSpeedMS;
}

double GameObject::CalculateSpawnTime(const double time) const
{
    double low = time - TermiaConfig::GetInstance()->GetScrollSpeedMS();
    double high = time;

    // Expand backward until the object is far enough away.
    for (int i = 0; i < 100 && GetVisualDistance(time, low) < 1.0; ++i)
    {
        const double interval = high - low;
        high = low;
        low -= interval;
    }

    // Binary search.
    for (int i = 0; i < 60; ++i)
    {
        const double mid = (low + high) * 0.5;

        if (GetVisualDistance(time, mid) >= 1.0)
            low = mid;
        else
            high = mid;
    }

    return (low + high) * 0.5;
}

GameObject::GameObject(
    Chart* chart,
    const int lane,
    const double time,
    const int hitSound,
    const int type)
    : _lane(lane),
      _time(time),
      _hitSound(hitSound),
      _type(type),
      _chart(chart)
{
    _spawnTime = CalculateSpawnTime(time);
}

int GameObject::GetLane() const {
    return _lane;
}

int GameObject::GetTime() const {
    return _time;
}

int GameObject::GetHitSound() const {
    return _hitSound;
}

int GameObject::GetType() const {
    return _type;
}

double GameObject::GetSpawnTime() const {
    return _spawnTime;
}

void GameObject::Hit() {
    _hit=true;
}

bool GameObject::IsNote() const {
    return (_type & 0b00000001)>0;
}

bool GameObject::IsHold() const {
    return (_type & 0b10000000)>0;
}

bool GameObject::IsHit() const {
    return _hit;
}
