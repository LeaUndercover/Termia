//
// Created by cutie on 29.08.26.
//

#include "TermiaConfig.h"

TermiaConfig* TermiaConfig::_instance = nullptr;

void TermiaConfig::SetSpeed(float speed) {
    _speed=speed;
}

void TermiaConfig::SetVolume(float volume) {
    _volume=volume;
}

void TermiaConfig::SetScrollSpeed(float scrollSpeed) {
    _scrollSpeed=scrollSpeed;
}

TermiaConfig *TermiaConfig::GetInstance()
{
    if(_instance==nullptr){
        _instance = new TermiaConfig();
    }
    return _instance;
}

float TermiaConfig::GetSpeed() const {
    return _speed;
}

float TermiaConfig::GetScrollSpeed() const {
    return _scrollSpeed;
}

float TermiaConfig::GetScrollSpeedMS() const {
    return (13720 / _scrollSpeed)*_speed;
}

float TermiaConfig::GetVolume() const {
    return _volume;
}
