//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TUIMENU_H
#define TERMIA_TUIMENU_H
#include <string>
#include "Viewport.h"
#include "Game/InputEvent.h"

class TUIManager;

class TUIMenu {
protected:
    TUIManager& _manager;
public:
    virtual ~TUIMenu() = default;

    const Viewport &GetViewport();

    TUIMenu(TUIManager& manager): _manager(manager) {};

    virtual void onDraw() = 0;
    virtual void onKeyDown(InputEvent event) = 0;
    virtual void onKeyUp(InputEvent event) = 0;
    virtual std::string getName() = 0;
};


#endif //TERMIA_TUIMENU_H
