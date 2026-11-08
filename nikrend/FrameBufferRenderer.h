//
// Created by cutie on 19.10.26.
//

#ifndef TERMIA_FRAMEBUFFERRENDERER_H
#define TERMIA_FRAMEBUFFERRENDERER_H
#include "Renderer.h"


class FrameBufferRenderer: public Renderer {
protected:
    std::vector<std::vector<char>> _charBuffer;

    void rasterizeTri(std::vector<Vector2> triPoints);
    void writeAlignedLine(const std::string &str, int y, const Alignment &alignment);
    void printInBuffer(int y, int x, const std::string &str);
    void drawLine(Vector2 p1, Vector2 p2, const char *ch);
    void clearFrameBuffer();
public:
    FrameBufferRenderer(const Menu_Play &play, const Viewport &viewport): Renderer(play,viewport) {
        clearFrameBuffer();
    };

    std::vector<std::string> GetFrameBuffer() override;
};


#endif //TERMIA_FRAMEBUFFERRENDERER_H