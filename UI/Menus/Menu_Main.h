//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_MAIN_H
#define TERMIA_MENU_MAIN_H
#include "../TUIMenu.h"


class Menu_Main final : public TUIMenu {
private:
    int _selection = 0;

public:
    Menu_Main(TUIManager& manager): TUIMenu(manager) {};

    std::string getName() override;
    void onDraw() override;
    void onKeyDown(InputEvent event) override;
    void onKeyUp(InputEvent event) override;
};


#endif //TERMIA_MENU_MAIN_H