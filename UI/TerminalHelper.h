//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TERMINALHELPER_H
#define TERMIA_TERMINALHELPER_H
#include <string>

#include "Viewport.h"


class TerminalHelper {
public:
    static void DrawLine(const Viewport& viewport, std::ostringstream &frame);
    static void WriteCenteredLine(const Viewport& viewport, std::ostringstream &frame, const std::string &text);
};


#endif //TERMIA_TERMINALHELPER_H