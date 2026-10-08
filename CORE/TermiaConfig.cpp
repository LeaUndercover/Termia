#include "TermiaConfig.h"

#include <utility>
using namespace CORE;

TermiaConfig* TermiaConfig::_instance = nullptr;

void TermiaConfig::SetSpeed(const float speed) {
    _speed=speed;
}

void TermiaConfig::SetVolume(const float volume) {
    _volume=volume;
}

void TermiaConfig::SetScrollSpeed(const float scrollSpeed) {
    _scrollSpeed=scrollSpeed;
}

void TermiaConfig::SetKeys(std::vector<char> keys) {
    _keys=std::move(keys);
}

void TermiaConfig::SetQuitKey(int key) {
    _keyQuit = key;
}

void TermiaConfig::SetSearchPath(std::string path) {
    _searchPath=std::move(path);
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

std::vector<char> TermiaConfig::GetKeys() const {
    return _keys;
}

char TermiaConfig::GetQuitKey() const {
    return _keyQuit;
}

std::string TermiaConfig::GetSearchPath() const {
    return _searchPath;
}
