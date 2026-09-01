//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TUIMENU_H
#define TERMIA_TUIMENU_H
#include <string>


class TUIMenu {
public:
    virtual ~TUIMenu() = default;
    virtual void onDraw() = 0;
    virtual void onKeyDown(int key) = 0;
    virtual void onKeyUp(int key) = 0;
    virtual std::string getName() = 0;
};


#endif //TERMIA_TUIMENU_H