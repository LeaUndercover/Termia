//
// Created by cutie on 19.10.26.
//

#ifndef TERMIA_NIKRENDERER3D_H
#define TERMIA_NIKRENDERER3D_H
#include "FrameBufferRenderer.h"
#include "Geometry/Mesh.h"
#include "Geometry/Vector3.h"


class NikRenderer3D: public FrameBufferRenderer {
private:
    Vector3 _cameraPos;
    Vector3 _cameraRot;

    Vector2 worldToScreen(Vector2 v);
    float calcMinZ(int vertYOffset);
    double getNoteWidth() const;
    void rasterizeProjectedFace(std::vector<Vector2> points);
    void drawObject(Mesh mesh);
    void drawObjects();
    void drawLaneDividers();
    void drawHits();
    void drawHitBanner();
    void drawOffset();

public:
    NikRenderer3D(const Menu_Play &play, const Viewport &viewport): FrameBufferRenderer(play,viewport), _cameraPos(0,2,0), _cameraRot(-0.75,0,0) {};

    void OnDraw() override;
};


#endif //TERMIA_NIKRENDERER3D_H