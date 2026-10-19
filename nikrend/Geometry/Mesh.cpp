//
// Created by cutie on 19.10.26.
//

#include "Mesh.h"

void Mesh::AddFace(const std::vector<int>& face) {
    _faces.push_back(face);
}

std::vector<Vector3> Mesh::GetVertices() {
    return _vertices;
}

std::vector<std::vector<int>> Mesh::GetFaces() {
    return _faces;
}

void Mesh::AddVertice(const Vector3 &vertex) {
    _vertices.push_back(vertex);
}
