//
// Created by cutie on 28.08.26.
//

#include "Menu_Options.h"

#include <iostream>
#include <sstream>

#include "Menu_Main.h"
#include "../../CORE/TermiaConfig.h"
#include "../ConsoleConstants.h"
#include "../TerminalHelper.h"
#include "../TUIManager.h"
using namespace CORE;

std::string Menu_Options::getName() {
    return "Options";
}

std::vector<std::string> KeycodesToString(const std::vector<char> &keycodes) {
    std::vector<std::string> result;

    for (const int keycode: keycodes)
        result.emplace_back(1,keycode);

    return result;
}

std::string join(const std::vector<std::string>& strings, const char separator) {
    std::string result;

    for (const auto& s : strings) {
        if (!result.empty())
            result += separator;
        result += s;
    }

    return result;
}

std::string wrap(const std::string& str, const std::string& left, const std::string& right) {
    return left+str+right;
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

    auto keycodeStrings = KeycodesToString(config->GetKeys());
    if (_selected==3) {
        if (_subSelected>3 || _subSelected<0)
            _subSelected = 0;

        keycodeStrings[_subSelected] = wrap(keycodeStrings[_subSelected],"[","]");
    }
    frame << "KEYS " + join(keycodeStrings,' ') << std::endl;

    auto quitKey = std::string(1,config->GetQuitKey());
    if (_selected==4)
        frame << "QUIT KEY " + wrap(quitKey,"[","]") << std::endl;
    else
        frame << "QUIT KEY " + quitKey << std::endl;

    if (_selected==5)
        frame << "SEARCHPATH " + wrap(config->GetSearchPath(),"[","]") << std::endl;
    else
        frame << "SEARCHPATH " + config->GetSearchPath() << std::endl;

    auto consoleKey = std::string(1,config->GetConsoleKey());
    if (_selected==6)
        frame << "CONSOLE KEY " + wrap(consoleKey,"[","]") << std::endl;
    else
        frame << "CONSOLE KEY " + consoleKey << std::endl;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_Options::onKeyDown(InputEvent event) {
    auto config = TermiaConfig::GetInstance();
    auto keyCode = event.GetKeyCode();
    auto key = std::tolower(event.GetParsedChar());

    if (key==config->GetQuitKey())
        TUIManager::GetInstance()->ChangeMenu(std::make_unique<Menu_Main>());

    if (keyCode==KEY_DOWN)
    {
        _subSelected=0;
        _selected++;
        return;
    }

    if (keyCode==KEY_UP)
    {
        _subSelected=0;
        _selected--;
        return;
    }

    if (keyCode==KEY_LEFT) {
        if (_subSelected>0)
            _subSelected--;

        if (_selected==0&&config->GetSpeed()>=0.55)
            config->SetSpeed(config->GetSpeed()-0.05);

        if (_selected==1&&config->GetScrollSpeed()>=1)
            config->SetScrollSpeed(config->GetScrollSpeed()-0.5);

        if (_selected==2&&config->GetVolume()>=0.05)
            config->SetVolume(config->GetVolume()-0.05);
        return;
    }

    if (keyCode==KEY_RIGHT) {
        _subSelected++;

        if (_selected==0&&config->GetSpeed()<=1.95)
            config->SetSpeed(config->GetSpeed()+0.05);

        if (_selected==1&&config->GetScrollSpeed()<=40)
            config->SetScrollSpeed(config->GetScrollSpeed()+0.5);

        if (_selected==2&&config->GetVolume()<=1.95)
            config->SetVolume(config->GetVolume()+0.05);
        return;
    }

    if (_selected==3) {
        auto keys = config->GetKeys();
        keys[_subSelected] = key;
        config->SetKeys(keys);
    }

    if (_selected==4)
        config->SetQuitKey(key);

    if (_selected==5) {
        auto searchPath = config->GetSearchPath();

        if (keyCode==KEY_ENTER)
            return;

        if (keyCode==KEY_BACKSPACE)
            searchPath.pop_back();
        else
            searchPath+=key;

        config->SetSearchPath(searchPath);
    }

    if (_selected==6)
        config->SetConsoleKey(key);
}

void Menu_Options::onKeyUp(InputEvent event) {}