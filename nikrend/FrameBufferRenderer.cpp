//
// Created by cutie on 19.10.26.
//

#include "FrameBufferRenderer.h"

#include <cassert>
#include <stdexcept>

#include "Geometry/BoundingBox.h"

// assumes points are screen space & z is pre culled
void FrameBufferRenderer::rasterizeTri(std::vector<Vector2> triPoints) {
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

void FrameBufferRenderer::writeAlignedLine(const std::string &str, const int y, const Alignment& alignment) {
    auto size = _viewport.GetSize();

    if (alignment == left)
        printInBuffer(y,0,str);

    if (alignment == center)
        printInBuffer(y, std::round((size.getWidth() - str.length()) / 2) + 1, str);

    // think + 1 is needed to align because my renderer is aligned one to the right :sob:
    if (alignment == right)
        printInBuffer(y, size.getWidth() - 1 - str.length(), str);
}

void FrameBufferRenderer::printInBuffer(const int y, const int x, const std::string &str) {
    const auto size = _viewport.GetSize();

    for (const auto c:str)
        _charBuffer[y][x] = c;
}

void FrameBufferRenderer::clearFrameBuffer() {
    const auto size = _viewport.GetSize();
    _charBuffer=std::vector<std::vector<char>>(size.getHeight(), std::vector<char>(size.getWidth(), ' '));
}

void FrameBufferRenderer::drawLine(const Vector2 p1, const Vector2 p2, const char *ch) {
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

std::vector<std::string> FrameBufferRenderer::GetFrameBuffer() {
    const auto size = _viewport.GetSize();
    std::vector<std::string> frameBuffer;

    for (int y = 0; y < size.getHeight(); ++y) {
        std::string line;

        for (int x = 0; x < size.getWidth(); ++x)
            line += _charBuffer[y][x];

        frameBuffer.emplace_back(line);
    }

    return frameBuffer;
}
