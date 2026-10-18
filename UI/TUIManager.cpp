//
// Created by cutie on 28.08.26.
//

#include "TUIManager.h"
#include <iostream>
#include <linux/input.h>

#include "Game/InputManager.h"

TUIManager* TUIManager::_instance = nullptr;

TUIManager *TUIManager::GetInstance()
{
    if(_instance==nullptr){
        _instance = new TUIManager();
    }
    return _instance;
}

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

void TUIManager::SetResolution(const std::pair<unsigned int, unsigned int> &res) {
    _resolution = res;
}

std::pair<unsigned int, unsigned int> TUIManager::GetResolution() const {
    return _resolution;
}
