//
// Created by cutie on 08.10.26.
//

#ifndef TERMIA_INPUTEVENT_H
#define TERMIA_INPUTEVENT_H
#include <bits/types.h>
#include <bits/types/struct_timeval.h>


class InputEvent {
private:
    struct timeval _time;
    __uint16_t _keyCode;
    __int32_t _value;
    char _parsedChar;

public:
    InputEvent(const struct timeval time, const __uint16_t keyCode, const __int32_t value, const char parsedChar) : _time(time), _keyCode(keyCode), _value(value), _parsedChar(parsedChar) {};
    struct timeval GetTime() const;
    __uint16_t GetKeyCode() const;
    __int32_t GetValue() const;
    char GetParsedChar() const;
};


#endif //TERMIA_INPUTEVENT_H