//
// Created by cutie on 18.10.26.
//

#include "PlainNikRenderer.h"

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

void PlainNikRenderer::printInBuffer(const int y, const int x, const std::string &str) {
    for (const auto c:str)
        _charBuffer[y][x] = c;
}

// assumes points are screen space & z is pre culled
void PlainNikRenderer::rasterizeTri(std::vector<Vector2> triPoints) {
    assert(triPoints.size() == 3);

    // rounding needed to match to line stuff
    for (auto& p : triPoints) {
        p.x = std::round(p.x);
        p.y = std::round(p.y);
    }

    const auto boundingBox = BoundingBox(_viewport,triPoints);

    Vector2 plocal;
    for (int x = static_cast<int>(std::ceil(boundingBox.vmin.x)); x <= static_cast<int>(std::floor(boundingBox.vmax.x)); x++) {
        plocal.x = x;

        for (int y = static_cast<int>(std::ceil(boundingBox.vmin.y)); y <= static_cast<int>(std::floor(boundingBox.vmax.y)); y++) {
            plocal.y = y;

            const double alpha = Vector2::EdgefuncP(plocal, triPoints[1], triPoints[2]);
            const double beta  = Vector2::EdgefuncP(plocal, triPoints[2], triPoints[0]);
            const double gamma = Vector2::EdgefuncP(plocal, triPoints[0], triPoints[1]);

            const bool hasNegative = alpha < 0 || beta < 0 || gamma < 0;
            const bool hasPositive = alpha > 0 || beta > 0 || gamma > 0;

            if (hasNegative && hasPositive)
                continue;

            printInBuffer(y, x, "#");
        }
    }
}

// assumes quad :p
void PlainNikRenderer::rasterizeProjectedFace(const std::vector<Vector2> &points) {
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

void PlainNikRenderer::drawOffset() {
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

void PlainNikRenderer::drawLaneDividers() {
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

void PlainNikRenderer::drawHits() {
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

void PlainNikRenderer::drawObjects() {
    const double noteWidth = getNoteWidth();
    const auto curTime = _play.GetCurTime();

    for (const auto object: _gameObjects) {
        const auto lane = object->GetLane() + 1;
        const auto size = _viewport.GetSize();

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

void PlainNikRenderer::writeAlignedLine(const std::string &str, const int y, const Alignment& alignment) {
    auto size = _viewport.GetSize();

    if (alignment == left)
        printInBuffer(y,0,str);

    if (alignment == center)
        printInBuffer(y, std::round((size.getWidth() - str.length()) / 2) + 1, str);

    // think + 1 is needed to align because my renderer is aligned one to the right :sob:
    if (alignment == right)
        printInBuffer(y, size.getWidth() - 1 - str.length(), str);
}
double PlainNikRenderer::getNoteWidth() const {
    const auto size = _viewport.GetSize();

    const double noteWidth = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * static_cast<double>(size.getWidth())) / 4.0f;

    return noteWidth;
}

void PlainNikRenderer::drawLine(const Vector2 p1, const Vector2 p2, const char *ch) {
    const double dx = fabs(p1.x - p2.x);
    const double dy = fabs(p1.y - p2.y);

    const auto size = _viewport.GetSize();
    const double steps = (dx > dy) ? dx : dy;

    if (steps != 0) {
        const double step = 1 / steps;

        for (double t = 0; t < 1; t += step) {
            const Vector2 linePos(
                std::round(p1.x + t * (p2.x - p1.x)),
                std::round(p1.y + t * (p2.y - p1.y))
            );

            // check for non pre culled objects -> can probably be made obselete
            if (linePos.x < size.getWidth() - 1 && linePos.y < size.getHeight() && linePos.x >= 0 && linePos.y >= 0)
                printInBuffer(linePos.y, linePos.x, ch);
        }
    }
    else
        throw std::runtime_error("Divide by zero in line drawing function");
}

void PlainNikRenderer::drawHitBanner() {
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

std::vector<std::string> PlainNikRenderer::GetFrameBuffer() {
    const auto size = _viewport.GetSize();
    _charBuffer=std::vector<std::vector<char>>(size.getHeight(), std::vector<char>(size.getWidth(), ' '));

    drawLaneDividers();
    drawObjects();
    drawOffset();
    drawHitBanner();
    drawHits();

    std::vector<std::string> frameBuffer;

    for (int y = 0; y < size.getHeight(); ++y) {
        std::string line;

        for (int x = 0; x < size.getWidth(); ++x)
            line += _charBuffer[y][x];

        frameBuffer.emplace_back(line);
    }

    return frameBuffer;
}