#include <iostream>

#include "CORE/Chart.h"
#include "Game/AudioManager.h"
#include "UI/Menus/Menu_Main.h"
#include "UI/TUIManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    //std::string path = "/mnt/stuff/termia/518951 LeaF - Aleph-0/";
    //std::string file = "LeaF - Aleph-0 (jakads) [Cardinality].osu";
    //CORE::Chart test = CORE::Chart(path,file);
    //return 0;

    auto uiManager = TUIManager::GetInstance();
    auto mainMenu = Menu_Main();

    //uiManager->SetResolution({80,28});
    uiManager->SetResolution({320,60});
    uiManager->ChangeMenu(std::make_unique<Menu_Main>());

    while (true)
        uiManager->DoEvents();

    return 0;
}
