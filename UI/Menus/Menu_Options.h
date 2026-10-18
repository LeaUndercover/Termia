//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_OPTIONS_H
#define TERMIA_MENU_OPTIONS_H
#include "../TUIMenu.h"


class Menu_Options : public TUIMenu {
private:
    unsigned _selected = 0;
    unsigned _subSelected = 0;
public:
    std::string getName() override;
    void onDraw() override;
    void onKeyDown(InputEvent event) override;
    void onKeyUp(InputEvent event) override;
};


#endif //TERMIA_MENU_OPTIONS_H