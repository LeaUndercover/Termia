//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_SONGSELECT_H
#define TERMIA_MENU_SONGSELECT_H
#include <filesystem>

#include "../../CORE/Chart.h"
#include "../TUIMenu.h"


class Menu_SongSelect : public TUIMenu {
private:
    unsigned _selection = 0;
    void onPathChange();
    void onHover();
public:
    //Menu_SongSelect(): TUIMenu(), _path(std::filesystem::current_path()) { onPathChange(); };
    Menu_SongSelect(): TUIMenu() {
        onPathChange();
        onHover();
    };
    std::string getName() override;
    void onDraw() override;
    void onKeyDown(InputEvent event) override;
    void onKeyUp(InputEvent event) override;
};


#endif //TERMIA_MENU_SONGSELECT_H