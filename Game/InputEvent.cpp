//
// Created by cutie on 08.10.26.
//

#include "InputEvent.h"

struct timeval InputEvent::GetTime() const {
    return _time;
}

__uint16_t InputEvent::GetKeyCode() const {
    return _keyCode;
}

__int32_t InputEvent::GetValue() const {
    return _value;
}

char InputEvent::GetParsedChar() const {
    return _parsedChar;
}
