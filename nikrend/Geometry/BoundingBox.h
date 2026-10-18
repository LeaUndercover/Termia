//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_BOUNDINGBOX_H
#define TERMIA_BOUNDINGBOX_H
#include <algorithm>

#include "Vector2.h"
#include "nikrend/Renderer.h"
#include "UI/Viewport.h"

class BoundingBox {
public:
    Vector2 vmin;
    Vector2 vmax;

    BoundingBox(const Viewport& viewport, const std::vector<Vector2>& points) {
        const auto size = viewport.GetSize();

        std::vector<double> xBuf;
        std::vector<double> yBuf;

        for (const auto& p : points) {
            xBuf.push_back(p.x);
            yBuf.push_back(p.y);
        }

        vmin.x = std::clamp(
            *std::ranges::min_element(xBuf),
            0.0,
            static_cast<double>(size.getWidth() - 1)
        );

        vmax.x = std::clamp(
            *std::ranges::max_element(xBuf),
            0.0,
            static_cast<double>(size.getWidth() - 1)
        );

        const double top = PLAYFIELD_VERT_START;
        const double bottom = size.getHeight() - PLAYFIELD_VERT_END - 1;

        vmin.y = std::clamp(
            *std::ranges::min_element(yBuf),
            top,
            bottom
        );

        vmax.y = std::clamp(
            *std::ranges::max_element(yBuf),
            top,
            bottom
        );
    }
};

#endif //TERMIA_BOUNDINGBOX_H