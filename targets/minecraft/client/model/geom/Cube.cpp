#include "Cube.h"

#include <array>
#include <span>

#include "ModelPart.h"
#include "minecraft/client/model/Polygon.h"
#include "minecraft/client/model/Vertex.h"

class Tesselator;

// void Cube::addBox(float x0, float y0, float z0, int w, int h, int d, float g)
Cube::Cube(ModelPart* modelPart, int xTexOffs, int yTexOffs, float x0, float y0,
           float z0, int w, int h, int d, float g, int faceMask /* = 63 */,
           bool bFlipPoly3UVs)
    :  // 4J - added faceMask, added bFlipPoly3UVs to reverse the uvs back so
       // player skins display right
      x0(x0),
      y0(y0),
      z0(z0),
      x1(x0 + w),
      y1(y0 + h),
      z1(z0 + d),
      vertices({Vertex{0, 0, 0, 0, 0}, Vertex{0, 0, 0, 0, 0},
                Vertex{0, 0, 0, 0, 0}, Vertex{0, 0, 0, 0, 0},
                Vertex{0, 0, 0, 0, 0}, Vertex{0, 0, 0, 0, 0},
                Vertex{0, 0, 0, 0, 0}, Vertex{0, 0, 0, 0, 0}}),
      polygons({}) {
    float x1 = x0 + w;
    float y1 = y0 + h;
    float z1 = z0 + d;

    x0 -= g;
    y0 -= g;
    z0 -= g;
    x1 += g;
    y1 += g;
    z1 += g;

    if (modelPart->bMirror) {
        float tmp = x1;
        x1 = x0;
        x0 = tmp;
    }

    const Vertex u0 = Vertex(x0, y0, z0, 0, 0);
    const Vertex u1 = Vertex(x1, y0, z0, 0, 8);
    const Vertex u2 = Vertex(x1, y1, z0, 8, 8);
    const Vertex u3 = Vertex(x0, y1, z0, 8, 0);

    const Vertex l0 = Vertex(x0, y0, z1, 0, 0);
    const Vertex l1 = Vertex(x1, y0, z1, 0, 8);
    const Vertex l2 = Vertex(x1, y1, z1, 8, 8);
    const Vertex l3 = Vertex(x0, y1, z1, 8, 0);

    vertices[0] = u0;
    vertices[1] = u1;
    vertices[2] = u2;
    vertices[3] = u3;
    vertices[4] = l0;
    vertices[5] = l1;
    vertices[6] = l2;
    vertices[7] = l3;

    // 4J - added ability to mask individual faces
    faceCount = 0;
    if (faceMask & 1)
        polygons[faceCount++] = _Polygon(
            std::array<const Vertex, 4>{l1, u1, u2, l2}, xTexOffs + d + w,
            yTexOffs + d, xTexOffs + d + w + d, yTexOffs + d + h,
            modelPart->xTexSize, modelPart->yTexSize);  // Right
    if (faceMask & 2)
        polygons[faceCount++] = _Polygon(
            std::array<const Vertex, 4>{u0, l0, l3, u3}, xTexOffs + 0,
            yTexOffs + d, xTexOffs + d, yTexOffs + d + h, modelPart->xTexSize,
            modelPart->yTexSize);  // Left
    if (faceMask & 4)
        polygons[faceCount++] = _Polygon(
            std::array<const Vertex, 4>{l1, l0, u0, u1}, xTexOffs + d,
            yTexOffs + 0, xTexOffs + d + w, yTexOffs + d, modelPart->xTexSize,
            modelPart->yTexSize);  // Up
    if (bFlipPoly3UVs) {
        if (faceMask & 8)
            polygons[faceCount++] = _Polygon(
                std::array<const Vertex, 4>{u2, u3, l3, l2}, xTexOffs + d + w,
                yTexOffs + 0, xTexOffs + d + w + w, yTexOffs + d,
                modelPart->xTexSize, modelPart->yTexSize);  // Down
    } else {
        if (faceMask & 8)
            polygons[faceCount++] = _Polygon(
                std::array<const Vertex, 4>{u2, u3, l3, l2}, xTexOffs + d + w,
                yTexOffs + d, xTexOffs + d + w + w, yTexOffs + 0,
                modelPart->xTexSize, modelPart->yTexSize);  // Down
    }
    if (faceMask & 16)
        polygons[faceCount++] =
            _Polygon(std::array<const Vertex, 4>{u1, u0, u3, u2}, xTexOffs + d,
                     yTexOffs + d, xTexOffs + d + w, yTexOffs + d + h,
                     modelPart->xTexSize, modelPart->yTexSize);  // Front
    if (faceMask & 32)
        polygons[faceCount++] = _Polygon(
            std::array<const Vertex, 4>{l0, l1, l2, l3}, xTexOffs + d + w + d,
            yTexOffs + d, xTexOffs + d + w + d + w, yTexOffs + d + h,
            modelPart->xTexSize,
            modelPart->yTexSize);  // Back

    if (modelPart->bMirror) {
        for (unsigned int i = 0; i < polygons.size(); i++) polygons[i].mirror();
    }
}

void Cube::render(Tesselator* t, float scale) {
    for (int i = 0; i < faceCount; i++) {
        polygons[i].render(t, scale);
    }
}

Cube* Cube::setId(const std::string& id) {
    this->id = id;
    return this;
}
