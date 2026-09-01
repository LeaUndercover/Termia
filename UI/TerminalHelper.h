//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TERMINALHELPER_H
#define TERMIA_TERMINALHELPER_H
#include <string>


class TerminalHelper {
public:
    static void DrawLine(std::ostringstream &frame);
    static void WriteCenteredLine(std::ostringstream &frame, const std::string &text);
};


#endif //TERMIA_TERMINALHELPER_H