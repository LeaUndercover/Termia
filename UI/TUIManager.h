//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TUIMANAGER_H
#define TERMIA_TUIMANAGER_H
#include <cstring>
#include <memory>

#include "TUIMenu.h"
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <iostream>

class TUIManager {
private:
    TUIManager(): _menu(nullptr) {};

    static TUIManager* _instance;

    std::unique_ptr<TUIMenu> _menu;
    std::pair<unsigned int, unsigned int> _resolution;

public:
    TUIManager(TUIManager &other) = delete;
    void operator=(const TUIManager &) = delete;
    static TUIManager *GetInstance();

    void DoEvents() const;
    void ChangeMenu(std::unique_ptr<TUIMenu> menu);
    void SetResolution(const std::pair<unsigned int, unsigned int> &res);
    std::pair<unsigned int, unsigned int> GetResolution() const;
};

#endif //TERMIA_TUIMANAGER_H