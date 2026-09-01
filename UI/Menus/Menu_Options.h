//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_OPTIONS_H
#define TERMIA_MENU_OPTIONS_H
#include "../TUIMenu.h"


class Menu_Options : public TUIMenu {
private:
    unsigned _selected = 0;
public:
    std::string getName() override;
    void onDraw() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;
};


#endif //TERMIA_MENU_OPTIONS_H