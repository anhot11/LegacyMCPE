#include "Vertex.h"

#include "minecraft/world/phys/Vec3.h"

Vertex Vertex::remap(const float u, const float v) const {
    return Vertex(pos.x, pos.y, pos.z, u, v);
}
