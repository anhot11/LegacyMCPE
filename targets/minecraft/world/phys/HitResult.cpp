#include "HitResult.h"

#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/phys/Vec3.h"

HitResult::HitResult(int x, int y, int z, int f, const Vec3& pos) {
    type = TILE;
    this->x = x;
    this->y = y;
    this->z = z;
    this->f = f;
    this->pos = pos;

    this->entity = nullptr;
}

HitResult::HitResult(std::shared_ptr<Entity> entity) {
    type = ENTITY;
    this->entity = entity;
    pos = {entity->x, entity->y, entity->z};

    x = y = z = f = 0;
}

double HitResult::distanceTo(std::shared_ptr<Entity> e) {
    double xd = pos.x - e->x;
    double yd = pos.y - e->y;
    double zd = pos.z - e->z;
    return xd * xd + yd * yd + zd * zd;
}
