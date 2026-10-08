//
// Created by cutie on 29.08.26.
//

#include "InputManager.h"
#include <xkbcommon/xkbcommon.h>

InputManager* InputManager::_instance = nullptr;

InputManager *InputManager::GetInstance()
{
    if(_instance==nullptr){
        _instance = new InputManager();
    }
    return _instance;
}

std::optional<InputEvent> InputManager::GetInput() {
    input_event ev;

    const int rc = libevdev_next_event(
            _dev,
            LIBEVDEV_READ_FLAG_NORMAL,
            &ev
        );

    if (rc == -EAGAIN)
        return std::nullopt;

    if (rc < 0)
        return std::nullopt;

    // +8 to account for evdev to XKB translation
    xkb_keycode_t keycode = ev.code + 8;

    if (ev.value == 1)
        xkb_state_update_key(_xkbState, keycode, XKB_KEY_DOWN);
    else
        xkb_state_update_key(_xkbState, keycode, XKB_KEY_UP);

    char text[5];
    int len = xkb_state_key_get_utf8(
        _xkbState,
        keycode,
        text,
        sizeof(text)
    );

    InputEvent event = InputEvent(ev.time,ev.code,ev.value,text[0]);

    return event;
}
