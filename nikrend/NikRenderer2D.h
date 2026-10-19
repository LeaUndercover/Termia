//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_PLAINRENDERER_H
#define TERMIA_PLAINRENDERER_H
#include "FrameBufferRenderer.h"
#include "Renderer.h"


class NikRenderer2D final : public FrameBufferRenderer {
private:
    std::vector<std::vector<char>> _charBuffer;
    void rasterizeProjectedFace(const std::vector<Vector2> &points);
    void drawOffset();
    void drawLaneDividers();
    void drawHits();
    void drawObjects();
    double getNoteWidth() const;
    void drawHitBanner();

public:
    NikRenderer2D(const Menu_Play &play, const Viewport &viewport): FrameBufferRenderer(play,viewport) {};

    void OnDraw() override;
};


#endif //TERMIA_PLAINRENDERER_H