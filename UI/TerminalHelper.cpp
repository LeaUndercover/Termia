//
// Created by cutie on 28.08.26.
//

#include "TerminalHelper.h"
#include <sstream>

#include "TUIManager.h"

void TerminalHelper::DrawLine(std::ostringstream &frame) {
    auto [width, height] = TUIManager::GetInstance()->GetResolution();

    for (int i = 0; i < width; ++i)
        frame << "-";

    frame << std::endl;
}

void TerminalHelper::WriteCenteredLine(std::ostringstream &frame, const std::string &text) {
    auto [width, height] = TUIManager::GetInstance()->GetResolution();

    for (int i = 0; i < width/2-text.length()/2; ++i)
        frame << " ";

    frame << text << std::endl;
}
