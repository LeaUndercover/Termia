//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_INPUTMANAGER_H
#define TERMIA_INPUTMANAGER_H
#include <cstring>
#include <memory>

#include <xkbcommon/xkbcommon.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <iostream>

#include "InputEvent.h"

class InputManager {
private:
    libevdev* _dev;
    int _devFd;
    xkb_context* _xkbContext = nullptr;
    xkb_keymap*  _xkbKeymap = nullptr;
    xkb_state*   _xkbState = nullptr;

    InputManager() {
        _devFd = open("/dev/input/event5", O_RDONLY | O_NONBLOCK); // TODO: Remove Hardcode

        if (_devFd < 0) {
            perror("open");
            exit(1);
        }

        _dev = nullptr;

        int rc = libevdev_new_from_fd(_devFd, &_dev);

        if (rc < 0) {
            std::cerr << "libevdev_new_from_fd failed: "
                      << std::strerror(-rc) << '\n';
            close(_devFd);
            exit(1);
        }

        libevdev_set_clock_id(_dev, CLOCK_MONOTONIC);

        _xkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

        const char* rules   = "evdev";
        const char* model   = "pc105";
        const char* layout  = "us";
        const char* variant = "";
        const char* options = "";

        xkb_rule_names names {
            rules,
            model,
            layout,
            variant,
            options
        };

        _xkbKeymap = xkb_keymap_new_from_names(
            _xkbContext,
            &names,
            XKB_KEYMAP_COMPILE_NO_FLAGS
        );

        _xkbState = xkb_state_new(_xkbKeymap);
    };

    ~InputManager() {
        libevdev_free(_dev);
        close(_devFd);
    }

    static InputManager* _instance;

public:
    InputManager(InputManager &other) = delete;
    void operator=(const InputManager &) = delete;
    static InputManager *GetInstance();

    std::optional<InputEvent> GetInput();
};


#endif
