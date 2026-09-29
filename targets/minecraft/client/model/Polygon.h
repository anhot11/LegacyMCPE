#pragma once

#include <span>
#include <vector>

#include "Vertex.h"
#include "minecraft/client/renderer/Tesselator.h"

class Tesselator;

class _Polygon {
public:
    std::vector<Vertex> vertices;
    int vertexCount;

private:
    bool _flipNormal;

public:
    _Polygon() = default;
    _Polygon(std::span<const Vertex> vertices);
    _Polygon(std::span<const Vertex, 4> vertices, int u0, int v0, int u1,
             int v1, float xTexSize, float yTexSize);
    _Polygon(std::span<const Vertex, 4> vertices, float u0, float v0, float u1,
             float v1);
    void mirror();
    void render(Tesselator* t, float scale);
    _Polygon* flipNormal();
};
