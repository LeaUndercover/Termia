//
// Created by cutie on 19.10.26.
//

#include "NikRenderer3D.h"

#include <cassert>
#include <iomanip>
#include <ios>

#include "nikrend_datatypes.h"
#include "CORE/Hold.h"
#include "CORE/Note.h"
#include "Geometry/Mesh.h"
#include "Geometry/Vector3.h"
#include "UI/Menus/Menu_Play.h"

Vector2 NikRenderer3D::worldToScreen(Vector2 v) {
    const auto size = _viewport.GetSize();
    
    v.x = (v.x + 1) / 2 * size.getWidth();
    v.y = (v.y + 1) / 2 * size.getHeight();
    return v;
}

float NikRenderer3D::calcMinZ(const int vertYOffset) {
    const auto size = _viewport.GetSize();
    
    float targetY = size.getHeight() - vertYOffset + 1;
    // SHIT IMPLEMENTATION FOR REUCING SEARCH VOLUME
    // 5 for current cam settings -> assumes we basically already know the answer #STUPID
    float searchDivider = 5;
    Vector3 cullVert{
        0,
        NOTE_HEIGHT_OFFSET,
        PLAYFIELD_LENGTH,
    };
    Vector2 cullProj = worldToScreen(cullVert.ProjectVector(_cameraPos,_cameraRot));
    for (float z = PLAYFIELD_LENGTH / searchDivider; cullProj.y <= targetY; z -= 0.1) {
        cullVert.z = z;
        cullProj = worldToScreen(cullVert.ProjectVector(_cameraPos,_cameraRot));
    }
    return cullVert.z;
}

double NikRenderer3D::getNoteWidth() const {
    const auto size = _viewport.GetSize();

    const double noteWidth = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * static_cast<double>(size.getWidth())) / 4.0f;

    return noteWidth;
}

void NikRenderer3D::rasterizeProjectedFace(std::vector<Vector2> points) {
    std::vector<Vector2> tris;

    points[0].x++;
    points[1].x++;

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

void NikRenderer3D::drawObject(Mesh mesh) {
    auto verts = mesh.GetVertices();
    auto faces = mesh.GetFaces();

    for (size_t face = 0; face < faces.size(); face++) {
        bool shouldCull = false;
        for (size_t pindex = 0; pindex < faces[face].size(); pindex++) {
            float minZ = calcMinZ(PLAYFIELD_VERT_END);
            if (verts[faces[face][pindex] - 1].z < minZ) {
                verts[faces[face][pindex] - 1].z = minZ;
                shouldCull = true;
                break;
            }
        }

        if (shouldCull == true)
            continue;

        std::vector<Vector2> projectedFace;
        projectedFace.clear();
        for (size_t pindex = 0; pindex < faces[face].size(); pindex++) {
            Vector2 curPoint = worldToScreen(verts[faces[face][pindex] - 1].ProjectVector(_cameraPos,_cameraRot));

            projectedFace.push_back(curPoint);
        }

        rasterizeProjectedFace(projectedFace);
    }
}

void NikRenderer3D::drawObjects() {
    const double noteWidth = getNoteWidth();
    const auto curTime = _play.GetCurTime();
    const auto size = _viewport.GetSize();

    for (const auto object: _gameObjects) {
        const auto lane = object->GetLane() + 1;
        Mesh mesh;

        float widthFactor = NOTE_WIDTH_FACTOR;
        float noteWidth3D = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * widthFactor / 4.0f);
        float minZ = calcMinZ(PLAYFIELD_VERT_END);

        if (object->IsNote()) {
            auto* note = dynamic_cast<const CORE::Note*>(object);
            const double noteStartPosition = 1-note->GetPosition(curTime);
            float noteZPos = (1 - cbrt(noteStartPosition)) * (minZ + PLAYFIELD_LENGTH);

            Vector3 nLeft = {
                (lane - 3) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                noteZPos,
            };
            Vector3 nRight = {
                (lane - 2) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                noteZPos,
            };

            if (noteZPos < minZ) {
                nLeft.z = minZ;
                nRight.z = minZ;
            }

            Vector2 projLeft = worldToScreen(nLeft.ProjectVector(_cameraPos,_cameraRot));
            projLeft.x++;
            Vector2 projRight = worldToScreen(nRight.ProjectVector(_cameraPos,_cameraRot));

            drawLine(projLeft, projRight, "X");
        }

        if (object->IsHold()) {
            auto* hold = dynamic_cast<const CORE::Hold*>(object);

            auto [startFactor, endFactor] = hold->GetPosition(curTime);
            startFactor = 1.0 - startFactor;
            endFactor = 1.0 - endFactor;

            const double playfieldHeight = size.getHeight() - PLAYFIELD_VERT_START - PLAYFIELD_VERT_END;
            const double holdStartPos = 1-startFactor;
            const double holdEndPos = PLAYFIELD_VERT_START + endFactor * playfieldHeight;

            float holdZStartpos = (1.0 - cbrt(startFactor)) * (minZ + PLAYFIELD_LENGTH);
            if (holdStartPos > 1)
                holdZStartpos = 0;

            float holdZEndpos = (1.0 - cbrt(endFactor)) * (minZ + PLAYFIELD_LENGTH);
            if (holdEndPos < 0)
                holdZEndpos = PLAYFIELD_LENGTH;

            Vector3 nLeftStart = {
                (lane - 3) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                holdZStartpos,
            };
            Vector3 nRightStart = {
                (lane - 2) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                holdZStartpos,
            };
            Vector3 nLeftEnd = {
                (lane - 3) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                holdZEndpos,
            };
            Vector3 nRightEnd = {
                (lane - 2) * noteWidth3D,
                NOTE_HEIGHT_OFFSET,
                holdZEndpos,
            };

            if (nLeftStart.z < minZ) {
                nLeftStart.z = minZ;
                nRightStart.z = minZ;
            }

            mesh.AddVertice(nLeftStart);
            mesh.AddVertice(nLeftEnd);
            mesh.AddVertice(nRightEnd);
            mesh.AddVertice(nRightStart);

            std::vector<int> face;
            face.insert(face.end(), {1, 2, 3, 4});
            mesh.AddFace(face);
            drawObject(mesh);

            Vector2 projLeft = worldToScreen(nLeftStart.ProjectVector(_cameraPos,_cameraRot));
            projLeft.x++;
            Vector2 projRight = worldToScreen(nRightStart.ProjectVector(_cameraPos,_cameraRot));

            if (holdStartPos <= 1)
                drawLine(projLeft, projRight, "X");
        }
    }
}

void NikRenderer3D::drawLaneDividers() {
    const float widthFactor = NOTE_WIDTH_FACTOR;
    const float noteWidth3D = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * widthFactor / 4.0f);

    const float minZ = calcMinZ(PLAYFIELD_VERT_END);

    Vector3 pStart = {
        0,
        0,
        PLAYFIELD_LENGTH,
    };
    Vector3 pEnd = {
        0,
        NOTE_HEIGHT_OFFSET,
        minZ,
    };
    for (int divider = 1; divider < 6; divider++) {
        pStart.x = (divider - 3) * noteWidth3D;
        pEnd.x = (divider - 3) * noteWidth3D;
        drawLine(worldToScreen(pStart.ProjectVector(_cameraPos,_cameraRot)), worldToScreen(pEnd.ProjectVector(_cameraPos,_cameraRot)), "*");
    }
}

void NikRenderer3D::drawHits() {
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

        float noteWidth3D = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * NOTE_WIDTH_FACTOR / 4.0f);
        float minZ = calcMinZ(PLAYFIELD_VERT_END);

        Vector3 pStart = {
            0,
            NOTE_HEIGHT_OFFSET,
            minZ,
        };
        Vector3 pEnd = {
            0,
            NOTE_HEIGHT_OFFSET,
            minZ,
        };

        pStart.x = (lane - 2) * noteWidth3D;
        pEnd.x = (lane - 1) * noteWidth3D;

        Vector2 proj_p_start = worldToScreen(pStart.ProjectVector(_cameraPos,_cameraRot));
        Vector2 proj_p_end = worldToScreen(pEnd.ProjectVector(_cameraPos,_cameraRot));

        proj_p_start.x += 1;
        proj_p_start.y += 1;
        proj_p_end.x -= 1;
        proj_p_end.y += 1;

        drawLine(proj_p_start, proj_p_end, ch);
    }
}

void NikRenderer3D::drawHitBanner() {
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

void NikRenderer3D::drawOffset() {
    auto size = _viewport.GetSize();

    float noteWidth3D = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * NOTE_WIDTH_FACTOR / 4.0f);
    float zMin = calcMinZ(PLAYFIELD_VERT_END);

    Vector3 p = {
        0,
        0,
        zMin,
    };

    for (int i = 0; i < 4; i++) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << _lastOffset[i];
        std::string str = stream.str();

        p.x = (i - 1) * noteWidth3D;
        Vector2 pProj = worldToScreen(p.ProjectVector(_cameraPos,_cameraRot));
        printInBuffer(size.getHeight() - PLAYFIELD_VERT_END + 3, pProj.x - str.size(), str);
    }
}

void NikRenderer3D::OnDraw() {
    clearFrameBuffer();

    drawLaneDividers();
    drawObjects();
    drawOffset();
    drawHitBanner();
    drawHits();
}