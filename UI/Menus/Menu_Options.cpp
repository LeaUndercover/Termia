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

// Warcrime
int keycode_to_char(int keycode) {
    switch (keycode) {
        // Letters
        case KEY_A: return 'a';
        case KEY_B: return 'b';
        case KEY_C: return 'c';
        case KEY_D: return 'd';
        case KEY_E: return 'e';
        case KEY_F: return 'f';
        case KEY_G: return 'g';
        case KEY_H: return 'h';
        case KEY_I: return 'i';
        case KEY_J: return 'j';
        case KEY_K: return 'k';
        case KEY_L: return 'l';
        case KEY_M: return 'm';
        case KEY_N: return 'n';
        case KEY_O: return 'o';
        case KEY_P: return 'p';
        case KEY_Q: return 'q';
        case KEY_R: return 'r';
        case KEY_S: return 's';
        case KEY_T: return 't';
        case KEY_U: return 'u';
        case KEY_V: return 'v';
        case KEY_W: return 'w';
        case KEY_X: return 'x';
        case KEY_Y: return 'y';
        case KEY_Z: return 'z';

        // Numbers
        case KEY_0: return '0';
        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';

        // Whitespace / control
        case KEY_SPACE:  return ' ';
        case KEY_TAB:    return '\t';
        case KEY_ENTER:  return '\n';
        case KEY_BACKSPACE: return '\b';

        // Punctuation
        case KEY_MINUS:      return '-';
        case KEY_EQUAL:      return '=';
        case KEY_LEFTBRACE:  return '[';
        case KEY_RIGHTBRACE: return ']';
        case KEY_BACKSLASH:  return '\\';
        case KEY_SEMICOLON:  return ';';
        case KEY_APOSTROPHE: return '\'';
        case KEY_GRAVE:      return '`';
        case KEY_COMMA:      return ',';
        case KEY_DOT:        return '.';
        case KEY_SLASH:      return '/';

        // Keypad
        case KEY_KP0: return '0';
        case KEY_KP1: return '1';
        case KEY_KP2: return '2';
        case KEY_KP3: return '3';
        case KEY_KP4: return '4';
        case KEY_KP5: return '5';
        case KEY_KP6: return '6';
        case KEY_KP7: return '7';
        case KEY_KP8: return '8';
        case KEY_KP9: return '9';

        case KEY_KPDOT:   return '.';
        case KEY_KPPLUS:  return '+';
        case KEY_KPMINUS: return '-';
        case KEY_KPASTERISK: return '*';
        case KEY_KPSLASH: return '/';

        default:
            return '\0';
    }
}

std::vector<std::string> KeycodesToString(const std::vector<int> &keycodes) {
    std::vector<std::string> result;

    for (const int keycode : keycodes) {
        result.emplace_back(1,keycode_to_char(keycode));
    }

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

    auto quitKey = std::string(1,keycode_to_char(config->GetQuitKey()));
    if (_selected==4)
        frame << "QUIT KEY " + wrap(quitKey,"[","]") << std::endl;
    else
        frame << "QUIT KEY " + quitKey << std::endl;

    if (_selected==5)
        frame << "SEARCHPATH " + wrap(config->GetSearchPath(),"[","]") << std::endl;
    else
        frame << "SEARCHPATH " + config->GetSearchPath() << std::endl;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_Options::onKeyDown(const int key) {
    auto config = TermiaConfig::GetInstance();

    if (key==KEY_Q)
        TUIManager::GetInstance()->ChangeMenu(std::make_unique<Menu_Main>());

    if (key==KEY_DOWN)
    {
        _subSelected=0;
        _selected++;
        return;
    }

    if (key==KEY_UP)
    {
        _subSelected=0;
        _selected--;
        return;
    }

    if (key==KEY_LEFT) {
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

    if (key==KEY_RIGHT) {
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

        if (key==KEY_ENTER)
            return;

        if (key==KEY_BACKSPACE)
            searchPath.pop_back();
        else
            searchPath+=keycode_to_char(key);

        config->SetSearchPath(searchPath);
    }
}

void Menu_Options::onKeyUp(const int key) {}