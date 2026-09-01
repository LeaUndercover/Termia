//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_MENU_PLAY_H
#define TERMIA_MENU_PLAY_H
#include <chrono>

#include "../Game/AudioManager.h"
#include "../Game/Chart.h"
#include "../Game/TermiaConfig.h"
#include "../UI/TUIMenu.h"


class Menu_Play: public TUIMenu {
private:
    std::chrono::time_point<std::chrono::steady_clock> _lastTick;
    Chart* _chart;
    double _curTime = 0;
    int _noteThickness = 18;
    char _noteChar = '_';
    char _hitChar = 'X';

    double _fps = 0;
    double _score = 0;
    double _avgOffset = 0;
    double _avgOffsetTotal = 0;
    double _hits = 0;
    double _combo = 0;
    std::vector<int> _lastOffset;

    std::vector<bool> _isPressed;
    std::vector<Hold*> _holding;

    void onMiss(GameObject* object);
    void onHit(GameObject* object);

    void onReleaseHit(Hold *hold);

    void onGameTick();
public:
    Menu_Play(Chart* chart) : _chart(chart), _isPressed(4, false), _lastOffset(4, 0), _holding(4, nullptr), _lastTick(std::chrono::steady_clock::now()) {
        auto audioManager = AudioManager::GetInstance();
        auto config = TermiaConfig::GetInstance();
        audioManager->PlaySound(_chart->GetPath() + "/" +_chart->GetAudioFileName(), config->GetSpeed());
    };

    std::string getName() override;
    void onDraw() override;
    void onKeyDown(int button) override;
    void onKeyUp(int key) override;
};


#endif //TERMIA_MENU_PLAY_H