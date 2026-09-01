//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_MAIN_H
#define TERMIA_MENU_MAIN_H
#include "../TUIMenu.h"


class Menu_Main : public TUIMenu {
private:
    int _selection = 0;

public:
    std::string getName() override;
    void onDraw() override;
    void onKeyDown(int button) override;
    void onKeyUp(int button) override;
};


#endif //TERMIA_MENU_MAIN_H