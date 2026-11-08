//
// Created by cutie on 28.08.26.
//

#include "TUIManager.h"
#include <iostream>
#include <linux/input.h>

#include "Game/InputManager.h"

void TUIManager::DoEvents() const {
    while (true) {
        _menu->onDraw();

        auto inputManager = InputManager::GetInstance();

        auto input = inputManager->GetInput();

        if (input==std::nullopt)
            continue;

        if (input->GetValue() == 1)
            _menu->onKeyDown(input.value());
        else if (input->GetValue() == 0)
            _menu->onKeyUp(input.value());
    }
}

void TUIManager::ChangeMenu(std::unique_ptr<TUIMenu> menu) {
    _menu = std::move(menu);
}

const Viewport & TUIManager::GetViewport() const {
    return _viewport;
}
