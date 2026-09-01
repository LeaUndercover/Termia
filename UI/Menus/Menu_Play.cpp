//
// Created by cutie on 28.08.26.
//

#include "Menu_Play.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Menu_Main.h"
#include "../../Game/Hold.h"
#include "../../Game/TermiaConfig.h"
#include "../ConsoleConstants.h"
#include "../TerminalHelper.h"
#include "../TUIManager.h"

void Menu_Play::onMiss(GameObject *object) {
    _combo=0;

    object->Hit();

    if (object->IsHold())
        _holding[object->GetLane()]=nullptr;
}

void Menu_Play::onHit(GameObject *object) {
    auto delta = object->GetTime()-_curTime;
    _score+=GameObject::GetScore(delta,_chart->GetOD());
    _combo++;
    _hits++;

    if (object->IsNote())
        object->Hit();
    else
        _holding[object->GetLane()]=static_cast<Hold*>(object);
}

void Menu_Play::onReleaseHit(Hold *hold) {
    auto releaseDelta = hold->GetTime()+hold->GetLength()-_curTime;
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

std::string Menu_Play::getName() {
    return "Play";
}

void Menu_Play::onDraw() {
    auto config = TermiaConfig::GetInstance();
    onGameTick();

    auto [width,height] = TUIManager::GetInstance()->GetResolution();
    std::ostringstream frame;

    TerminalHelper::DrawLine(frame);
    TerminalHelper::WriteCenteredLine(frame, "[Playing " + _chart->GetName() + "] @ " + std::to_string(_curTime));
    TerminalHelper::DrawLine(frame);

    int scrollSpeedMS = config->GetScrollSpeedMS();

    auto visible = _chart->GetVisibleObjects(_curTime,scrollSpeedMS);

    int playPositions = height-5;
    for (int i = 0; i < playPositions; i++) {
        std::vector<bool> hasNote(4,false);

        for (const auto object:visible)
            if (object->IsNote()) {
                auto* note = static_cast<Note*>(object);
                if (note->QuantizePosition(_curTime,scrollSpeedMS,playPositions)==i)
                    hasNote[note->GetLane()]=true;
            }
            else
            if (object->IsHold()) {
                auto* hold = static_cast<Hold*>(object);
                auto [start,end] = hold->QuantizePosition(_curTime,scrollSpeedMS,playPositions);
                if (start>=i&&end<=i)
                    hasNote[hold->GetLane()]=true;
            }


        frame   << (hasNote[0]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[1]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[2]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[3]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' '));

        if (i==1)
            frame << std::string(20,' ') << "FPS: " << _fps;

        if (i==2)
            frame << std::string(20,' ') << "Score: " << _score;

        if (i==3)
            frame << std::string(20,' ') << "AvgMS: " << _avgOffset;

        if (i==4)
            frame << std::string(20,' ') << "Combo: " << _combo;

        if (i==5)
            frame << std::string(20,' ') << "Hits: " << _hits;

        if (i==6)
            frame << std::string(20,' ') << "OD: " << _chart->GetOD();

        frame << std::endl;
    }

    frame   << (_isPressed[0]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[1]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[2]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[3]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << std::endl;

    frame   << std::fixed << std::setw(_noteThickness) << _lastOffset[0] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[1] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[2] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[3];

    frame << std::defaultfloat;

    std::cout << CONSOLE_CLEAR;
    std::cout << frame.str();
    std::cout.flush();

    auto now = std::chrono::steady_clock::now();
    double deltaTimeMs = std::chrono::duration<double, std::milli>(now - _lastTick).count();
    _fps=1000/deltaTimeMs;
    _curTime+=deltaTimeMs*config->GetSpeed();
    _lastTick = std::chrono::steady_clock::now();
}

void Menu_Play::onKeyDown(const int key) {
    auto od = _chart->GetOD();
    if (key==KEY_S) // TODO: Fix Hardcoding
    {
        _isPressed[0]=true;
        auto nearestObj = _chart->GetNearestObject(0,_curTime);
        auto delta = nearestObj->GetTime()-_curTime;
        _lastOffset[0]=delta;

        // Check if inside hittable window
        if (GameObject::IsInHitWindow(delta,od)) {
            auto score = GameObject::GetScore(delta,od);
            if (score!=0)
                onHit(nearestObj);
            else
                onMiss(nearestObj);

            _avgOffsetTotal+=delta;
            _avgOffset=_avgOffsetTotal/_hits;
        }
    }

    if (key==KEY_D) {
        _isPressed[1]=true;
        auto nearestObj = _chart->GetNearestObject(1,_curTime);
        auto delta = nearestObj->GetTime()-_curTime;
        _lastOffset[1]=delta;

        // Check if inside hittable window
        if (GameObject::IsInHitWindow(delta,od)) {
            auto score = GameObject::GetScore(delta,od);
            if (score!=0)
                onHit(nearestObj);
            else
                onMiss(nearestObj);

            _avgOffsetTotal+=delta;
            _avgOffset=_avgOffsetTotal/_hits;
        }
    }

    if (key==KEY_K)
    {
        _isPressed[2]=true;
        auto nearestObj = _chart->GetNearestObject(2,_curTime);
        auto delta = nearestObj->GetTime()-_curTime;
        _lastOffset[2]=delta;

        // Check if inside hittable window
        if (GameObject::IsInHitWindow(delta,od)) {
            auto score = GameObject::GetScore(delta,od);
            if (score!=0)
                onHit(nearestObj);
            else
                onMiss(nearestObj);

            _avgOffsetTotal+=delta;
            _avgOffset=_avgOffsetTotal/_hits;
        }
    }

    if (key==KEY_L)
    {
        _isPressed[3]=true;
        auto nearestObj = _chart->GetNearestObject(3,_curTime);
        auto delta = nearestObj->GetTime()-_curTime;
        _lastOffset[3]=delta;

        // Check if inside hittable window
        if (GameObject::IsInHitWindow(delta,od)) {
            auto score = GameObject::GetScore(delta,od);
            if (score!=0)
                onHit(nearestObj);
            else
                onMiss(nearestObj);

            _avgOffsetTotal+=delta;
            _avgOffset=_avgOffsetTotal/_hits;
        }
    }

    if (key==KEY_Q) {
        AudioManager::GetInstance()->StopSounds();
        TUIManager::GetInstance()->ChangeMenu(std::make_unique<Menu_Main>());
    }
}

void Menu_Play::onKeyUp(const int key) {
    auto od = _chart->GetOD();

    if (key==KEY_S) // TODO: Fix Hardcoding
    {
        _isPressed[0]=false;
        auto nearestHold = _chart->GetNearestReleasableHold(0,_curTime);
        if (nearestHold) {
            auto releaseDelta = nearestHold->GetTime()+nearestHold->GetLength()-_curTime;

            if (GameObject::IsInHitWindow(releaseDelta,od)) {
                auto score = GameObject::GetScore(releaseDelta,od);
                if (score!=0)
                    onReleaseHit(nearestHold);
                else
                    onMiss(nearestHold);

                _avgOffsetTotal+=releaseDelta;
                _avgOffset=_avgOffsetTotal/_hits;
            }
            else
                if (_holding[0]!=nullptr)
                    onMiss(_holding[0]);
        }
    }

    if (key==KEY_D) {
        _isPressed[1]=false;
        auto nearestHold = _chart->GetNearestReleasableHold(1,_curTime);
        if (nearestHold) {
            auto releaseDelta = nearestHold->GetTime()+nearestHold->GetLength()-_curTime;

            if (GameObject::IsInHitWindow(releaseDelta,od)) {
                auto score = GameObject::GetScore(releaseDelta,od);
                if (score!=0)
                    onReleaseHit(nearestHold);
                else
                    onMiss(nearestHold);

                _avgOffsetTotal+=releaseDelta;
                _avgOffset=_avgOffsetTotal/_hits;
            }
            else
                if (_holding[1]!=nullptr)
                    onMiss(_holding[1]);
        }
    }

    if (key==KEY_K)
    {
        _isPressed[2]=false;
        auto nearestHold = _chart->GetNearestReleasableHold(2,_curTime);
        if (nearestHold) {
            auto releaseDelta = nearestHold->GetTime()+nearestHold->GetLength()-_curTime;

            if (GameObject::IsInHitWindow(releaseDelta,od)) {
                auto score = GameObject::GetScore(releaseDelta,od);
                if (score!=0)
                    onReleaseHit(nearestHold);
                else
                    onMiss(nearestHold);

                _avgOffsetTotal+=releaseDelta;
                _avgOffset=_avgOffsetTotal/_hits;
            }
            else
                if (_holding[2]!=nullptr)
                    onMiss(_holding[2]);
        }
    }

    if (key==KEY_L)
    {
        _isPressed[3]=false;
        auto nearestHold = _chart->GetNearestReleasableHold(3,_curTime);
        if (nearestHold) {
            auto releaseDelta = nearestHold->GetTime()+nearestHold->GetLength()-_curTime;

            if (GameObject::IsInHitWindow(releaseDelta,od)) {
                auto score = GameObject::GetScore(releaseDelta,od);
                if (score!=0)
                    onReleaseHit(nearestHold);
                else
                    onMiss(nearestHold);

                _avgOffsetTotal+=releaseDelta;
                _avgOffset=_avgOffsetTotal/_hits;
            }
            else
                if (_holding[3]!=nullptr)
                    onMiss(_holding[3]);
        }
    }
}