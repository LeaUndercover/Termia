//
// Created by cutie on 28.08.26.
//

#include "Menu_Main.h"

#include <iostream>
#include <sstream>

#include "Menu_Options.h"
#include "Menu_SongSelect.h"
#include "../UI/ConsoleConstants.h"
#include "../UI/TerminalHelper.h"
#include "../UI/TUIManager.h"

std::string Menu_Main::getName() {
    return "Main Menu";
}

void Menu_Main::onDraw() {
    std::ostringstream frame;

    TerminalHelper::DrawLine(frame);
    TerminalHelper::WriteCenteredLine(frame, "[TERMIA]");
    TerminalHelper::DrawLine(frame);

    frame << (_selection==0?"[0]":" 0 ") << " Play" << std::endl;
    frame << (_selection==1?"[1]":" 1 ") << " Options" << std::endl;
    frame << (_selection==2?"[2]":" 2 ") << " Exit" << std::endl;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_Main::onKeyDown(const int key) {
    if (key==KEY_DOWN) {
        _selection++;
    }
    if (key==KEY_UP) {
        _selection--;
    }

    if (key==KEY_ENTER) {
        auto uiManager = TUIManager::GetInstance();
        switch (_selection) {
            case 0:
                uiManager->ChangeMenu(std::make_unique<Menu_SongSelect>());
                break;
            case 1:
                uiManager->ChangeMenu(std::make_unique<Menu_Options>());
                break;
            case 2:
                exit(0);
                break;
        }
    }
}

void Menu_Main::onKeyUp(const int button) {}
