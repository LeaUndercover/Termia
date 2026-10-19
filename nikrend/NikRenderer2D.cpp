//
// Created by cutie on 18.10.26.
//

#include "NikRenderer2D.h"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "CORE/Hold.h"
#include "CORE/Note.h"
#include "Geometry/BoundingBox.h"
#include "Geometry/Vector2.h"
#include "UI/Menus/Menu_Play.h"

// assumes quad :p
void NikRenderer2D::rasterizeProjectedFace(const std::vector<Vector2> &points) {
    std::vector<Vector2> tris;

    if (points.size() > 3) {
        const std::vector<Vector2> tri1{
            points[0],
            points[1],
            points[2],
        };
        const std::vector<Vector2> tri2{
            points[0],
            points[2],
            points[3],
        };
        rasterizeTri(tri1);
        rasterizeTri(tri2);
    }
    else
    {
        const std::vector<Vector2> tri{
            points[0],
            points[1],
            points[2],
        };
        rasterizeTri(tri);
    }
}

void NikRenderer2D::drawOffset() {
    const auto size = _viewport.GetSize();
    const double noteWidth = getNoteWidth();

    for (int i = 0; i < 4; i++) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << _lastOffset[i];
        std::string str = stream.str();

        printInBuffer(size.getHeight() - PLAYFIELD_VERT_END + 2,
            (REL_PLAYFIELD_START * static_cast<double>(size.getWidth()) + (i + 1) * noteWidth + 1) - str.size() - 1,
            str);
    }
}

void NikRenderer2D::drawLaneDividers() {
    const auto size = _viewport.GetSize();
    const double noteWidth = getNoteWidth();

    Vector2 p1;
    p1.y = static_cast<double>(PLAYFIELD_VERT_START);

    Vector2 p2;
    p2.y = size.getHeight() - static_cast<double>(PLAYFIELD_VERT_END) + 0.5; // why does adding 1 add two???????

    for (int divider_index = 0; divider_index <= 4; divider_index++) {
        const int xPos = std::round(REL_PLAYFIELD_START * static_cast<double>(size.getWidth()) + divider_index * noteWidth);
        p1.x = xPos;
        p2.x = xPos;
        drawLine(p1, p2, "|");
    }
}

void NikRenderer2D::drawHits() {
    double noteWidth = getNoteWidth();
    const auto size = _viewport.GetSize();

    for (int lane = 0; lane < KEY_SIZE; lane++) {
        if (!_isPressed[lane])
            continue;

        const char *ch;
        if (_lastOffset[lane] == 0)
            ch = ".";
        else if (_lastOffset[lane] < 0)
            ch = "-";
        else
            ch = "^";

        const double yPos = static_cast<double>(size.getHeight()) - PLAYFIELD_VERT_END + 1;
        const Vector2 p1 = Vector2(
            REL_PLAYFIELD_START * static_cast<double>(size.getWidth()) + lane * noteWidth + 1,
            yPos
        );
        const Vector2 p2 = Vector2(
            REL_PLAYFIELD_START * static_cast<double>(size.getWidth()) + (lane+1) * (noteWidth),
            yPos
        );

        drawLine(p1, p2, ch);
    }
}

void NikRenderer2D::drawObjects() {
    const double noteWidth = getNoteWidth();
    const auto curTime = _play.GetCurTime();
    const auto size = _viewport.GetSize();

    for (const auto object: _gameObjects) {
        const auto lane = object->GetLane() + 1;

        if (object->IsNote()) {
            auto* note = dynamic_cast<const CORE::Note*>(object);
            const double noteStartPosition = 1-note->GetPosition(curTime);
            const double objectYStartPos = PLAYFIELD_VERT_START + noteStartPosition * (size.getHeight() - PLAYFIELD_VERT_START - PLAYFIELD_VERT_END);
            if (noteStartPosition <= 1) {
                const Vector2 p1(
                    REL_PLAYFIELD_START * size.getWidth() + noteWidth * (lane - 1) + 1,
                    objectYStartPos
                );
                const Vector2 p2(
                    REL_PLAYFIELD_START * size.getWidth() + noteWidth * lane,
                    objectYStartPos
                );

                drawLine(p1, p2, "X");
            }
        }

        if (object->IsHold()) {
            auto* hold = dynamic_cast<const CORE::Hold*>(object);

            auto [startFactor, endFactor] = hold->GetPosition(curTime);
            startFactor = 1.0 - startFactor;
            endFactor = 1.0 - endFactor;

            const double playfieldHeight = size.getHeight() - PLAYFIELD_VERT_START - PLAYFIELD_VERT_END;
            const double holdYStartPos = PLAYFIELD_VERT_START + startFactor * playfieldHeight;
            const double holdYEndPos = PLAYFIELD_VERT_START + endFactor * playfieldHeight;

            const Vector2 leftStart{
                REL_PLAYFIELD_START * size.getWidth() + noteWidth * (lane - 1) + 1,
                holdYStartPos,
            };
            const Vector2 rightStart{
                (REL_PLAYFIELD_START * size.getWidth() + noteWidth * lane),
                holdYStartPos,
            };
            const Vector2 leftEnd{
                REL_PLAYFIELD_START * size.getWidth() + noteWidth * (lane - 1) + 1,
                holdYEndPos,
            };
            const Vector2 rightEnd{
                REL_PLAYFIELD_START * size.getWidth() + noteWidth * lane,
                holdYEndPos,
            };

            std::vector<Vector2> holdPoints = {leftStart, leftEnd, rightEnd, rightStart};

            rasterizeProjectedFace(holdPoints);

            if (startFactor <= 1)
                drawLine(leftStart, rightStart, "X");
        }
    }
}

double NikRenderer2D::getNoteWidth() const {
    const auto size = _viewport.GetSize();

    const double noteWidth = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * static_cast<double>(size.getWidth())) / 4.0f;

    return noteWidth;
}

void NikRenderer2D::drawHitBanner() {
    auto size = _viewport.GetSize();

    std::string hit_text;
    switch (_lastHit) {
        case 320:
            hit_text = " Perfect ";
            break;
        case 300:
            hit_text = " 300 ";
            break;
        case 200:
            hit_text = " 200 ";
            break;
        case 100:
            hit_text = " 100 ";
            break;
        case 50:
            hit_text = " 50 ";
            break;
        case 0:
            hit_text = " Miss ";
            break;
        default:
            hit_text = "";
    }
    writeAlignedLine(hit_text, size.getHeight() / 2, center);
}

void NikRenderer2D::OnDraw() {
    clearFrameBuffer();

    drawLaneDividers();
    drawObjects();
    drawOffset();
    drawHitBanner();
    drawHits();
}
