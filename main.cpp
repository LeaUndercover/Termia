#include <iostream>

#include "CORE/Chart.h"
#include "Game/AudioManager.h"
#include "UI/Menus/Menu_Main.h"
#include "UI/TUIManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    const auto size = Size2(320,60);
    const auto viewport = Viewport(size);
    TUIManager uiManager = TUIManager(viewport);
    auto mainMenu = Menu_Main(uiManager);
    uiManager.ChangeMenu(std::make_unique<Menu_Main>(uiManager));

    while (true)
        uiManager.DoEvents();

    return 0;
}
