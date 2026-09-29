#pragma once

#include <memory>

class Entity;

class EntityPos {
public:
    double x, y, z;
    float yRot, xRot;
    bool rot;
    bool move;

    EntityPos(double x, double y, double z, float yRot, float xRot);
    EntityPos(double x, double y, double z);
    EntityPos(float yRot, float xRot);
    EntityPos* lerp(std::shared_ptr<Entity> e, float f);
};