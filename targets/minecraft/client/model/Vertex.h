#pragma once
#include "minecraft/world/phys/Vec3.h"

class Vertex {
public:
    Vec3 pos;
    float u, v;

public:
    constexpr Vertex(float x, float y, float z, float u, float v)
        : pos({x, y, z}), u(u), v(v) {}

    constexpr Vertex(Vertex* vertex, float u, float v)
        : pos(vertex->pos), u(u), v(v) {}

    constexpr Vertex(Vec3* pos, float u, float v) : pos(*pos), u(u), v(v) {}

    Vertex remap(float u, float v) const;
};
