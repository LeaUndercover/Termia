#include <iostream>

#include "Game/AudioManager.h"
#include "Menus/Menu_Main.h"
#include "UI/TUIManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto uiManager = TUIManager::GetInstance();
    auto mainMenu = Menu_Main();

    //uiManager->SetResolution({80,28});
    uiManager->SetResolution({320,60});
    uiManager->ChangeMenu(std::make_unique<Menu_Main>());

    while (true)
        uiManager->DoEvents();

    return 0;
}
