#include "MushroomCow.h"

#include <memory>

#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/animal/Cow.h"
#include "minecraft/world/entity/item/ItemEntity.h"
#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/PlantTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

MushroomCow::MushroomCow(Level* level) : Cow(level) {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();
    setHealth(getMaxHealth());

    this->setSize(0.9f, 1.3f);
}

bool MushroomCow::mobInteract(std::shared_ptr<Player> player) {
    std::shared_ptr<ItemInstance> item = player->inventory->getSelected();
    if (item != nullptr && item->id == Item::bowl_Id && getAge() >= 0) {
        if (item->count == 1) {
            player->inventory->setItem(
                player->inventory->selected,
                std::shared_ptr<ItemInstance>(
                    new ItemInstance(Item::mushroomStew)));
            return true;
        }

        if (player->inventory->add(std::shared_ptr<ItemInstance>(
                new ItemInstance(Item::mushroomStew))) &&
            !player->abilities.instabuild) {
            player->inventory->removeItem(player->inventory->selected, 1);
            return true;
        }
    }
    // 4J: Do not allow shearing if we can't create more cows
    if (item != nullptr && item->id == Item::shears_Id && getAge() >= 0 &&
        level->canCreateMore(eTYPE_COW, Level::eSpawnType_Breed)) {
        remove();
        level->addParticle(eParticleType_largeexplode, x, y + bbHeight / 2, z,
                           0, 0, 0);
        if (!level->isClientSide) {
            remove();
            std::shared_ptr<Cow> cow = std::make_shared<Cow>(level);
            cow->moveTo(x, y, z, yRot, xRot);
            cow->setHealth(getHealth());
            cow->yBodyRot = yBodyRot;
            level->addEntity(cow);
            for (int i = 0; i < 5; i++) {
                level->addEntity(std::shared_ptr<ItemEntity>(
                    new ItemEntity(level, x, y + bbHeight, z,
                                   std::shared_ptr<ItemInstance>(
                                       new ItemInstance(Tile::mushroom_red)))));
            }
            return true;
        }
        return true;
    }
    return Cow::mobInteract(player);
}

// 4J - added so that mushroom cows have more of a chance of spawning, they can
// now spawn on mycelium as well as grass - seems a bit odd that they don't
// already really
bool MushroomCow::canSpawn() {
    int xt = Mth::floor(x);
    int yt = Mth::floor(bb.y0);
    int zt = Mth::floor(z);
    return (level->getTile(xt, yt - 1, zt) == Tile::grass_Id ||
            level->getTile(xt, yt - 1, zt) == Tile::mycel_Id) &&
           level->getDaytimeRawBrightness(xt, yt, zt) > 8 &&
           PathfinderMob::canSpawn();
}

std::shared_ptr<AgableMob> MushroomCow::getBreedOffspring(
    std::shared_ptr<AgableMob> target) {
    // 4J - added limit to number of animals that can be bred
    if (level->canCreateMore(GetType(), Level::eSpawnType_Breed)) {
        return std::make_shared<MushroomCow>(level);
    } else {
        return nullptr;
    }
}
