//
// Created by cutie on 29.08.26.
//

#include "GameObject.h"

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
