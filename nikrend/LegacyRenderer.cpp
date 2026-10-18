//
// Created by cutie on 18.10.26.
//

#include "LegacyRenderer.h"

#include <iomanip>

#include "CORE/Hold.h"
#include "CORE/Note.h"
#include "UI/Menus/Menu_Play.h"

std::vector<std::string> LegacyRenderer::GetFrameBuffer() {
    std::vector<std::string> frameBuffer;
    auto size = _viewport.GetSize();
    auto playPositions = size.getHeight()-5;
    const auto curTime = _play.GetCurTime();

    for (int i = 0; i < playPositions; i++) {
        std::stringstream line;
        std::vector<bool> hasNote(4,false);

        for (const auto object:_gameObjects)
            if (object->IsNote()) {
                auto* note = dynamic_cast<const CORE::Note*>(object);

                const auto startFactor = note->GetPosition(curTime);
                const int startPosition = playPositions - std::round(startFactor * playPositions);

                if (startPosition==i)
                    hasNote[note->GetLane()]=true;
            }
            else
            if (object->IsHold()) {
                auto* hold = dynamic_cast<const CORE::Hold*>(object);

                const auto positionFactors = hold->GetPosition(curTime);
                const int startPosition = playPositions - std::round(positionFactors.first * playPositions);
                const int endPosition = playPositions - std::round(positionFactors.second * playPositions);

                if (startPosition>=i&&endPosition<=i)
                    hasNote[hold->GetLane()]=true;
            }


        line    << (hasNote[0]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[1]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[2]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' ')) << "|"
                << (hasNote[3]?std::string(_noteThickness, _noteChar):std::string(_noteThickness, ' '));

        if (i==1)
            line << std::string(20,' ') << "FPS: " << _fps;

        if (i==2)
            line << std::string(20,' ') << "Score: " << _score;

        if (i==3)
            line << std::string(20,' ') << "AvgMS: " << _avgOffset;

        if (i==4)
            line << std::string(20,' ') << "Combo: " << _combo;

        if (i==5)
            line << std::string(20,' ') << "Hits: " << _hits;

        //if (i==6)
        //    line << std::string(20,' ') << "OD: " << _chart->GetOD();

        if (i==7)
            line << std::string(20,' ') << "VISIBLE: " << _gameObjects.size();

        //if (i==8)
        //    frame << std::string(20,' ') << "BPM: " << _chart->GetBPM(_curTime);

        frameBuffer.emplace_back(line.str());
    }

    std::stringstream line;

    line    << (_isPressed[0]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[1]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[2]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' ')) << "|"
            << (_isPressed[3]?std::string(_noteThickness, _hitChar):std::string(_noteThickness, ' '));

    frameBuffer.emplace_back(line.str());

    line=std::stringstream();

    line    << std::fixed << std::setw(_noteThickness) << _lastOffset[0] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[1] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[2] << "|"
            << std::fixed << std::setw(_noteThickness) << _lastOffset[3];

    line << std::defaultfloat;

    frameBuffer.emplace_back(line.str());

    return frameBuffer;
}
