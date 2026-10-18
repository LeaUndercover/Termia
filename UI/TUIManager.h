//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TUIMANAGER_H
#define TERMIA_TUIMANAGER_H
#include <memory>
#include "TUIMenu.h"

#include "Viewport.h"

class TUIManager {
private:
    std::unique_ptr<TUIMenu> _menu;
    Viewport _viewport;

public:
    TUIManager(const Viewport& viewport): _menu(nullptr), _viewport(viewport) {};

    void DoEvents() const;
    void ChangeMenu(std::unique_ptr<TUIMenu> menu);
    const Viewport& GetViewport() const;
};

#endif //TERMIA_TUIMANAGER_H