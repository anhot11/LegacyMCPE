#include "DeadBushTile.h"

#include <memory>

#include "minecraft/stats/GenericStats.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/PlantTile.h"
#include "minecraft/world/level/tile/Tile.h"

DeadBushTile::DeadBushTile(int id) : Bush(id, Material::replaceable_plant) {
    updateDefaultShape();
}

// 4J Added override
void DeadBushTile::updateDefaultShape() {
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 0.8f, 0.5f + ss);
}

bool DeadBushTile::mayPlaceOn(int tile) { return tile == Tile::sand_Id; }

int DeadBushTile::getResource(int data, Random* random, int playerBonusLevel) {
    return -1;
}

void DeadBushTile::playerDestroy(Level* level, std::shared_ptr<Player> player,
                                 int x, int y, int z, int data) {
    if (!level->isClientSide && player->getSelectedItem() != nullptr &&
        player->getSelectedItem()->id == Item::shears_Id) {
        player->awardStat(GenericStats::blocksMined(id),
                          GenericStats::param_blocksMined(id, data, 1));

        // drop leaf block instead of sapling
        popResource(level, x, y, z,
                    std::shared_ptr<ItemInstance>(
                        new ItemInstance(Tile::deadBush, 1, data)));
    } else {
        Bush::playerDestroy(level, player, x, y, z, data);
    }
}
