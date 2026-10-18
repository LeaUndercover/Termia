//
// Created by cutie on 18.10.26.
//

#ifndef TERMIA_RENDERER_H
#define TERMIA_RENDERER_H

#define REL_PLAYFIELD_START 0.3f // x distance from left to right
#define REL_PLAYFIELD_END 0.7f
#define PLAYFIELD_VERT_START 3 // starts on line 4 (because everything counts first line as line 0)
#define PLAYFIELD_VERT_END 4   // ends 3 lines before bottom

#define NOTE_WIDTH_FACTOR 10.0f
#define PLAYFIELD_LENGTH 40.0f
#define NOTE_HEIGHT_OFFSET 0.0f

#define KEY_SIZE 4

#include <vector>
#include "../CORE/GameObject.h"
#include "../UI/Viewport.h"
#include "Geometry/Alignment.h"
#include "Geometry/Vector2.h"

class Menu_Play;

class Renderer {
protected:
    const Menu_Play& _play;
    const Viewport& _viewport;
    signed int _lastHit;
    std::vector<const CORE::GameObject*> _gameObjects;

    int _noteThickness = 18;
    char _noteChar = '_';
    char _hitChar = 'X';

    double _fps = 0;
    double _score = 0;
    double _avgOffset = 0;
    double _avgOffsetTotal = 0;
    double _hits = 0;
    double _combo = 0;

    std::vector<bool> _isPressed;
    std::vector<int> _lastOffset;

public:
    virtual ~Renderer() = default;
    Renderer(const Menu_Play &play, const Viewport &viewport): _play(play), _viewport(viewport), _lastHit(0), _isPressed(KEY_SIZE, false), _lastOffset(KEY_SIZE, 0) {};

    void AddObjectToBuffer(const CORE::GameObject* object);
    void ClearObjectBuffer();
    void SetFPS(double fps);
    void SetScore(double score);
    void SetAvgOffset(double offset);
    void SetAvgOffsetTotal(double offset);
    void SetHits(double hits);
    void SetCombo(double combo);
    void SetPressed(const std::vector<bool> &pressed);
    void SetLastOffset(const std::vector<int> &offset);

    virtual std::vector<std::string> GetFrameBuffer() = 0;


};

#endif //TERMIA_RENDERER_H