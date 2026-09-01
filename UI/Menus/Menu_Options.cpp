//
// Created by cutie on 28.08.26.
//

#include "Menu_Options.h"

#include <iostream>
#include <sstream>

#include "Menu_Main.h"
#include "../../Game/TermiaConfig.h"
#include "../ConsoleConstants.h"
#include "../TerminalHelper.h"
#include "../TUIManager.h"

std::string Menu_Options::getName() {
    return "Options";
}

void Menu_Options::onDraw() {
    std::ostringstream frame;

    TerminalHelper::DrawLine(frame);
    TerminalHelper::WriteCenteredLine(frame, "[OPTIONS]");
    TerminalHelper::DrawLine(frame);

    auto config = TermiaConfig::GetInstance();

    if (_selected==0)
        frame << "[SPEED] " + std::to_string(config->GetSpeed()) << std::endl;
    else
        frame << "SPEED " + std::to_string(config->GetSpeed()) << std::endl;

    if (_selected==1)
        frame << "[SCROLLSPEED] " + std::to_string(config->GetScrollSpeed()) << std::endl;
    else
        frame << "SCROLLSPEED " + std::to_string(config->GetScrollSpeed()) << std::endl;

    if (_selected==2)
        frame << "[VOLUME] " + std::to_string(config->GetVolume()) << std::endl;
    else
        frame << "VOLUME " + std::to_string(config->GetVolume()) << std::endl;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_Options::onKeyDown(const int key) {
    auto config = TermiaConfig::GetInstance();

    if (key==KEY_Q)
        TUIManager::GetInstance()->ChangeMenu(std::make_unique<Menu_Main>());

    if (key==KEY_DOWN)
        _selected++;

    if (key==KEY_UP)
        _selected--;

    if (key==KEY_LEFT) {
        if (_selected==0&&config->GetSpeed()>=0.55)
            config->SetSpeed(config->GetSpeed()-0.05);

        if (_selected==1&&config->GetScrollSpeed()>=1)
            config->SetScrollSpeed(config->GetScrollSpeed()-0.5);

        if (_selected==2&&config->GetVolume()>=0.05)
            config->SetVolume(config->GetVolume()-0.05);
    }

    if (key==KEY_RIGHT) {
        if (_selected==0&&config->GetSpeed()<=1.95)
            config->SetSpeed(config->GetSpeed()+0.05);

        if (_selected==1&&config->GetScrollSpeed()<=40)
            config->SetScrollSpeed(config->GetScrollSpeed()+0.5);

        if (_selected==2&&config->GetVolume()<=1.95)
            config->SetVolume(config->GetVolume()+0.05);
    }
}

void Menu_Options::onKeyUp(const int key) {}