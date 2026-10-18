//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_PLAINRENDERER_H
#define TERMIA_PLAINRENDERER_H
#include "Renderer.h"


class PlainNikRenderer final : public Renderer {
private:
    std::vector<std::vector<char>> _charBuffer;

    void printInBuffer(int y, int x, const std::string &str);
    void rasterizeTri(std::vector<Vector2> triPoints);
    void rasterizeProjectedFace(const std::vector<Vector2> &points);
    void drawOffset();
    void drawLaneDividers();
    void drawHits();
    void drawObjects();
    void writeAlignedLine(const std::string &str, int y, const Alignment &alignment);
    double getNoteWidth() const;
    void drawLine(Vector2 p1, Vector2 p2, const char *ch);
    void drawHitBanner();

public:
    PlainNikRenderer(const Menu_Play& play, const Viewport& viewport) : Renderer(play,viewport) {};

    std::vector<std::string> GetFrameBuffer() override;
};


#endif //TERMIA_PLAINRENDERER_H