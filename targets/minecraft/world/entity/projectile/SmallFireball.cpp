#include "SmallFireball.h"

#include "minecraft/Facing.h"
#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/projectile/Fireball.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/HitResult.h"

SmallFireball::SmallFireball(Level* level) : Fireball(level) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

SmallFireball::SmallFireball(Level* level, std::shared_ptr<LivingEntity> mob,
                             double xa, double ya, double za)
    : Fireball(level, mob, xa, ya, za) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

SmallFireball::SmallFireball(Level* level, double x, double y, double z,
                             double xa, double ya, double za)
    : Fireball(level, x, y, z, xa, ya, za) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

void SmallFireball::onHit(HitResult* res) {
    if (!level->isClientSide) {
        if (res->entity != nullptr) {
            DamageSource* damageSource = DamageSource::fireball(
                std::dynamic_pointer_cast<Fireball>(shared_from_this()), owner);
            if (!res->entity->isFireImmune() &&
                res->entity->hurt(damageSource, 5)) {
                res->entity->setOnFire(5);
            }
            delete damageSource;
        } else {
            int tileX = res->x;
            int tileY = res->y;
            int tileZ = res->z;
            switch (res->f) {
                case Facing::UP:
                    tileY++;
                    break;
                case Facing::DOWN:
                    tileY--;
                    break;
                case Facing::NORTH:
                    tileZ--;
                    break;
                case Facing::SOUTH:
                    tileZ++;
                    break;
                case Facing::EAST:
                    tileX++;
                    break;
                case Facing::WEST:
                    tileX--;
                    break;
            };
            if (level->isEmptyTile(tileX, tileY, tileZ)) {
                level->setTileAndUpdate(tileX, tileY, tileZ, Tile::fire_Id);
            }
        }
        remove();
    }
}

bool SmallFireball::isPickable() { return false; }

bool SmallFireball::hurt(DamageSource* source, float damage) { return false; }