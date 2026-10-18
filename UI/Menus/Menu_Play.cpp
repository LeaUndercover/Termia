//
// Created by cutie on 28.08.26.
//

#include "Menu_Play.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Menu_Main.h"
#include "../../CORE/Hold.h"
#include "../../CORE/TermiaConfig.h"
#include "../ConsoleConstants.h"
#include "../TerminalHelper.h"
#include "../TUIManager.h"
#include "nikrend/PlainNikRenderer.h"

void Menu_Play::onMiss(GameObject *object) {
    _combo=0;

    object->Hit();

    if (object->IsHold())
        _holding[object->GetLane()]=nullptr;
}

void Menu_Play::onHit(GameObject *object, double delta) {
    _score+=GameObject::GetScore(delta,_chart->GetOD());
    _combo++;
    _hits++;

    if (object->IsNote())
        object->Hit();
    else
        _holding[object->GetLane()]=static_cast<Hold*>(object);
}

void Menu_Play::onReleaseHit(Hold *hold, double releaseDelta) {
    //auto releaseDelta = hold->GetTime()+hold->GetLength()-_curTime;
    _score+=GameObject::GetScore(releaseDelta,_chart->GetOD());
    _combo++;
    _hits++;

    hold->Hit();

    _holding[hold->GetLane()]=nullptr;
}

void Menu_Play::onGameTick() {
    auto& objects = _chart->GetObjects();
    auto od = _chart->GetOD();

    for (auto& object:objects) {
        if (object->IsTooLate(_curTime,od)) {
            object->Hit();
            onMiss(object.get());
        }
    }
}

Menu_Play::Menu_Play(TUIManager& manager,Chart *chart): TUIMenu(manager), _lastTick(std::chrono::steady_clock::now()), _chart(chart), _lastOffset(4, 0), _isPressed(4, false), _holding(4, nullptr) {
    _renderer=std::make_unique<PlainNikRenderer>(*this, _manager.GetViewport());

    auto audioManager = AudioManager::GetInstance();
    auto config = TermiaConfig::GetInstance();
    audioManager->PlaySound(_chart->GetPath() + "/" +_chart->GetAudioFileName(), config->GetSpeed());

    timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);

    _startTime.tv_sec = ts.tv_sec;
    _startTime.tv_usec = ts.tv_nsec / 1000;
}

std::string Menu_Play::getName() {
    return "Play";
}

void Menu_Play::onDraw() {
    auto config = TermiaConfig::GetInstance();
    onGameTick();

    std::ostringstream frame;

    TerminalHelper::DrawLine(_manager.GetViewport(),frame);
    TerminalHelper::WriteCenteredLine(_manager.GetViewport(),frame, "[Playing " + _chart->GetName() + "] @ " + std::to_string(_curTime));
    TerminalHelper::DrawLine(_manager.GetViewport(),frame);

    const auto visible = _chart->GetVisibleObjects(_curTime);

    _renderer->SetFPS(_fps);
    _renderer->SetAvgOffset(_avgOffset);
    _renderer->SetAvgOffsetTotal(_avgOffsetTotal);
    _renderer->SetHits(_hits);
    _renderer->SetCombo(_combo);
    _renderer->SetLastOffset(_lastOffset);
    _renderer->SetPressed(_isPressed);

    _renderer->ClearObjectBuffer();

    for (auto& object :visible)
        _renderer->AddObjectToBuffer(object);

    const auto frameBuffer = _renderer->GetFrameBuffer();

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    for (auto& line: frameBuffer)
        std::cout << line << std::endl;

    std::cout.flush();

    const auto now = std::chrono::steady_clock::now();
    const double deltaTimeMs = std::chrono::duration<double, std::milli>(now - _lastTick).count();
    _fps=1000/deltaTimeMs;

    _curTime += deltaTimeMs * config->GetSpeed();
    _lastTick = std::chrono::steady_clock::now();
}

void Menu_Play::onKeyDown(const InputEvent event) {
    auto config = TermiaConfig::GetInstance();
    auto od = _chart->GetOD();
    auto keys = config->GetKeys();
    auto key = event.GetParsedChar();
    auto keyCode = event.GetKeyCode();
    const int64_t eventTimeMS = static_cast<int64_t>(event.GetTime().tv_sec) * 1000 + event.GetTime().tv_usec / 1000;
    const int64_t startTimeMS = static_cast<int64_t>(_startTime.tv_sec) * 1000 + _startTime.tv_usec / 1000;
    const auto deltaTimeMS = eventTimeMS - startTimeMS;

    for (int i=0;i<keys.size();i++)
        if (key==keys.at(i))
        {
            _isPressed[i]=true;
            auto nearestObj = _chart->GetNearestObject(i,_curTime);
            auto delta = (nearestObj->GetTime()/config->GetSpeed())-deltaTimeMS;
            _lastOffset[i]=delta;

            // Check if inside hittable window
            if (GameObject::IsInHitWindow(delta,od)) {
                auto score = GameObject::GetScore(delta,od);
                if (score!=0)
                    onHit(nearestObj, delta);
                else
                    onMiss(nearestObj);

                _avgOffsetTotal+=delta;
                _avgOffset=_avgOffsetTotal/_hits;
            }
        }

    if (key==config->GetQuitKey())
    {
        AudioManager::GetInstance()->StopSounds();
        _manager.ChangeMenu(std::make_unique<Menu_Main>(_manager));
    }
}

void Menu_Play::onKeyUp(const InputEvent event) {
    auto config = TermiaConfig::GetInstance();
    auto od = _chart->GetOD();
    auto keys = config->GetKeys();
    auto key = event.GetParsedChar();
    auto keyCode = event.GetKeyCode();
    const int64_t eventTimeMS = static_cast<int64_t>(event.GetTime().tv_sec) * 1000 + event.GetTime().tv_usec / 1000;
    const int64_t startTimeMS = static_cast<int64_t>(_startTime.tv_sec) * 1000 + _startTime.tv_usec / 1000;
    const auto deltaTimeMS = eventTimeMS - startTimeMS;

    for (int i=0;i<keys.size();i++)
        if (key==keys.at(i))
        {
            _isPressed[i]=false;
            auto nearestHold = _chart->GetNearestReleasableHold(i,_curTime);
            if (nearestHold) {
                auto releaseDelta = (nearestHold->GetTime()/config->GetSpeed())+nearestHold->GetLength()-deltaTimeMS;

                if (GameObject::IsInHitWindow(releaseDelta,od)) {
                    auto score = GameObject::GetScore(releaseDelta,od);
                    if (score!=0)
                        onReleaseHit(nearestHold, releaseDelta);
                    else
                        onMiss(nearestHold);

                    _avgOffsetTotal+=releaseDelta;
                    _avgOffset=_avgOffsetTotal/_hits;
                }
                else
                    if (_holding[i]!=nullptr)
                        onMiss(_holding[i]);
            }
        }
}