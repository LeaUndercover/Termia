//
// Created by cutie on 28.08.26.
//

#include "Menu_SongSelect.h"

#include <iostream>
#include <sstream>

#include "Menu_Main.h"
#include "Menu_Play.h"
#include "../Game/ChartManager.h"
#include "../UI/ConsoleConstants.h"
#include "../UI/TerminalHelper.h"
#include "../UI/TUIManager.h"

void Menu_SongSelect::onPathChange() {
    auto chartManager = ChartManager::GetInstance();

    chartManager->Clear();

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{_path}) {
        if (!std::string(dir_entry.path().filename()).ends_with(".osu"))
            continue;

        chartManager->LoadChart(dir_entry.path().parent_path(), dir_entry.path().filename());
    }
}

void Menu_SongSelect::onHover() {
    auto audioManager = AudioManager::GetInstance();
    auto chartManager = ChartManager::GetInstance();
    auto config = TermiaConfig::GetInstance();

    audioManager->StopSounds();

    if (_selection >= 0 && _selection < chartManager->GetCharts().size()) {
        auto& chart = chartManager->GetCharts()[_selection];
        audioManager->PlaySound(chart->GetPath()+"/"+chart->GetAudioFileName(),config->GetSpeed(),chart->GetPreviewTime());
    }
}

std::string Menu_SongSelect::getName() {
    return "Song Select";
}

void Menu_SongSelect::onDraw() {
    std::ostringstream frame;

    TerminalHelper::DrawLine(frame);
    TerminalHelper::WriteCenteredLine(frame, "[Song Select]");
    TerminalHelper::DrawLine(frame);

    auto chartManager = ChartManager::GetInstance();

    unsigned index = 0;
    for (auto const& chart : chartManager->GetCharts()) {

        if (index==_selection)
            frame << "[" << chart->GetName() << "]" << std::endl;
        else
            frame << chart->GetName() << std::endl;

        index++;
    }

    TerminalHelper::DrawLine(frame);

    /*for (auto const& dir_entry : std::filesystem::directory_iterator{_path}) {
        if (!dir_entry.is_directory())
            continue;

        if (index==_selection)
            frame << "[" << dir_entry.path() << "]" << std::endl;
        else
            frame << dir_entry.path() << std::endl;

        index++;
    }*/

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();
}

void Menu_SongSelect::onKeyDown(const int key) {
    if (key==KEY_DOWN) {
        _selection++;
        onHover();
    }

    if (key==KEY_UP) {
        _selection--;
        onHover();
    }



    if (key==KEY_ENTER) {
        auto chartManager = ChartManager::GetInstance();
        auto audioManager = AudioManager::GetInstance();

        if (_selection >= 0 && _selection < chartManager->GetCharts().size()) {
            audioManager->StopSounds();
            TUIManager::GetInstance()->ChangeMenu(
                std::make_unique<Menu_Play>(chartManager->GetCharts()[_selection].get())
            );
        }
    }


    if (key==KEY_Q) {
        AudioManager::GetInstance()->StopSounds();
        TUIManager::GetInstance()->ChangeMenu(std::make_unique<Menu_Main>());
    }
}

void Menu_SongSelect::onKeyUp(const int key) {}