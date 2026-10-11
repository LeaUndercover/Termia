#include "GameObject.h"
using namespace CORE;

int GameObject::GetLane() const {
    return _lane;
}

int GameObject::GetTime() const {
    return _time+_offset;
}

int GameObject::GetTimeOffset() const {
    return _time+_offset;
}

int GameObject::GetHitSound() const {
    return _hitSound;
}

int GameObject::GetType() const {
    return _type;
}

double GameObject::GetOffset() const {
    return _offset;
}

void GameObject::SetOffset(const double offset) {
    _offset = offset;
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
