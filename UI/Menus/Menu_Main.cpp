//
// Created by cutie on 28.08.26.
//

#include "Menu_Main.h"

#include <iostream>
#include <sstream>

#include "Menu_Options.h"
#include "Menu_SongSelect.h"
#include "../ConsoleConstants.h"
#include "../TerminalHelper.h"
#include "../TUIManager.h"

std::string Menu_Main::getName() {
    return "Main Menu";
}

void Menu_Main::onDraw() {
    std::ostringstream frame;

    TerminalHelper::DrawLine(_manager.GetViewport(),frame);
    TerminalHelper::WriteCenteredLine(_manager.GetViewport(),frame, "[TERMIA]");
    TerminalHelper::DrawLine(_manager.GetViewport(),frame);

    frame << (_selection==0?"[0]":" 0 ") << " Play" << std::endl;
    frame << (_selection==1?"[1]":" 1 ") << " Options" << std::endl;
    frame << (_selection==2?"[2]":" 2 ") << " Exit" << std::endl;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_Main::onKeyDown(InputEvent event) {
    auto keyCode = event.GetKeyCode();
    auto key = event.GetParsedChar();

    if (keyCode==KEY_DOWN)
        _selection++;

    if (keyCode==KEY_UP)
        _selection--;

    if (keyCode==KEY_ENTER) {
        switch (_selection) {
            case 0:
                _manager.ChangeMenu(std::make_unique<Menu_SongSelect>(_manager));
                break;
            case 1:
                _manager.ChangeMenu(std::make_unique<Menu_Options>(_manager));
                break;
            case 2:
                exit(0);
                break;
        }
    }
}

void Menu_Main::onKeyUp(InputEvent event) {}
