//
// Created by cutie on 28.08.26.
//

#include "TerminalHelper.h"
#include <sstream>

#include "TUIManager.h"

void TerminalHelper::DrawLine(const Viewport& viewport, std::ostringstream &frame) {
    for (int i = 0; i < viewport.GetSize().getWidth(); ++i)
        frame << "-";

    frame << std::endl;
}

void TerminalHelper::WriteCenteredLine(const Viewport& viewport, std::ostringstream &frame, const std::string &text) {
    for (int i = 0; i < viewport.GetSize().getWidth()/2-text.length()/2; ++i)
        frame << " ";

    frame << text << std::endl;
}
