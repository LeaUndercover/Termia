//
// Created by cutie on 28.08.26.
//

#include "TUIManager.h"
#include <iostream>
#include <libevdev/libevdev.h>
#include <linux/input.h>

TUIManager* TUIManager::_instance = nullptr;

TUIManager *TUIManager::GetInstance()
{
    if(_instance==nullptr){
        _instance = new TUIManager();
    }
    return _instance;
}

void TUIManager::DoEvents() const {
    _menu->onDraw();
    input_event ev;

    while (true) {
        int rc = libevdev_next_event(
            _dev,
            LIBEVDEV_READ_FLAG_NORMAL,
            &ev
        );

        if (rc == -EAGAIN)
            break;

        if (rc < 0)
            break;

        if (ev.type != EV_KEY)
            continue;

        if (ev.value == 1) {
            _menu->onKeyDown(ev.code);
        }
        else if (ev.value == 0) {
            _menu->onKeyUp(ev.code);
        }
    }
}

void TUIManager::ChangeMenu(std::unique_ptr<TUIMenu> menu) {
    _menu = std::move(menu);
}

void TUIManager::SetResolution(const std::pair<unsigned int, unsigned int> &res) {
    _resolution = res;
}

std::pair<unsigned int, unsigned int> TUIManager::GetResolution() const {
    return _resolution;
}
