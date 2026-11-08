//
// Created by cutie on 18.10.26.
//

#include "Renderer.h"

#include <cassert>
#include <cmath>
#include <stdexcept>
#include "Geometry/Alignment.h"
#include "Geometry/BoundingBox.h"
#include "Geometry/Vector2.h"
#include "UI/Menus/Menu_Play.h"

void Renderer::AddObjectToBuffer(const GameObject* object) {
    _gameObjects.emplace_back(object);
}

void Renderer::ClearObjectBuffer() {
    _gameObjects.clear();
}

void Renderer::SetFPS(const double fps) {
    _fps = fps;
}

void Renderer::SetScore(const double score) {
    _score = score;
}

void Renderer::SetAvgOffset(const double offset) {
    _avgOffset = offset;
}

void Renderer::SetAvgOffsetTotal(const double offset) {
    _avgOffsetTotal = offset;
}

void Renderer::SetHits(const double hits) {
    _hits = hits;
}

void Renderer::SetCombo(const double combo) {
    _combo = combo;
}

void Renderer::SetPressed(const std::vector<bool> &pressed) {
    _isPressed = pressed;
}

void Renderer::SetLastOffset(const std::vector<int> &offset) {
    _lastOffset = offset;
}
