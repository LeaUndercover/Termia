//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_GAMEOBJECT_H
#define TERMIA_GAMEOBJECT_H
#include "cmath"

class GameObject {
protected:
    int _lane;
    double _time;
    int _hitSound;
    int _type;
    bool _hit = false;

public:
    GameObject(const int lane, const double time, const int hitSound, const int type): _lane(lane), _time(time), _hitSound(hitSound), _type(type) {};

    int GetLane() const;
    int GetTime() const;
    int GetHitSound() const;
    int GetType() const;

    void Hit();

    static bool IsInHitWindow(double delta, double od) {
        return delta<=188 - 3*od&&delta>=-(127 - 3*od);
    };

    static int GetScore(double delta, double od) {
        delta = abs(delta);

        if (delta<=16)
            return 320;

        if (delta<=64 - 3*od)
            return 300;

        if (delta<=97 - 3*od)
            return 200;

        if (delta<=127 - 3*od)
            return 100;

        if (delta<=151 - 3*od)
            return 50;

        if (delta<=188 - 3*od)
            return 0;

        return 0;
    };

    virtual bool IsTooLate(double curTime, double od) = 0;

    bool IsNote() const;
    bool IsHold() const;
    bool IsHit() const;
};


#endif //TERMIA_GAMEOBJECT_H