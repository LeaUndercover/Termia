//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_LEGACYRENDERER_H
#define TERMIA_LEGACYRENDERER_H
#include "Renderer.h"


class LegacyRenderer final : public Renderer {
private:

public:
    LegacyRenderer(const Menu_Play& play, const Viewport& viewport): Renderer(play,viewport) {};
    std::vector<std::string> GetFrameBuffer() override;
};


#endif //TERMIA_LEGACYRENDERER_H