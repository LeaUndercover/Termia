//
// Created by cutie on 19.10.26.
//

#ifndef TERMIA_MESH_H
#define TERMIA_MESH_H
#include <vector>

#include "Vector3.h"

class Mesh {
private:
    std::vector<Vector3> _vertices;
    std::vector<std::vector<int>> _faces;
public:
    void AddVertice(const Vector3 &vertex);
    void AddFace(const std::vector<int> &face);

    std::vector<Vector3> GetVertices();
    std::vector<std::vector<int>> GetFaces();
};

#endif //TERMIA_MESH_H