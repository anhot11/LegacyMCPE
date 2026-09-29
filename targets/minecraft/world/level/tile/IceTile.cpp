#include "IceTile.h"

#include <memory>
#include <string>

#include "minecraft/stats/GenericStats.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/food/FoodConstants.h"
#include "minecraft/world/item/enchantment/EnchantmentHelper.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LightLayer.h"
#include "minecraft/world/level/dimension/Dimension.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/HalfTransparentTile.h"
#include "minecraft/world/level/tile/Tile.h"

class ItemInstance;

IceTile::IceTile(int id)
    : HalfTransparentTile(id, "ice", Material::ice, false) {
    friction = 0.98f;
    setTicking(true);
}

int IceTile::getRenderLayer() { return 1; }

bool IceTile::shouldRenderFace(LevelSource* level, int x, int y, int z,
                               int face) {
    return HalfTransparentTile::shouldRenderFace(level, x, y, z, 1 - face);
}

void IceTile::playerDestroy(Level* level, std::shared_ptr<Player> player, int x,
                            int y, int z, int data) {
    player->awardStat(GenericStats::blocksMined(id),
                      GenericStats::param_blocksMined(id, data, 1));
    player->causeFoodExhaustion(FoodConstants::EXHAUSTION_MINE);

    if (isSilkTouchable() && EnchantmentHelper::hasSilkTouch(player)) {
        std::shared_ptr<ItemInstance> item = getSilkTouchItemInstance(data);
        if (item != nullptr) {
            popResource(level, x, y, z, item);
        }
    } else {
        if (level->dimension->ultraWarm) {
            level->removeTile(x, y, z);
            return;
        }

        int playerBonusLevel = EnchantmentHelper::getDiggingLootBonus(player);
        spawnResources(level, x, y, z, data, playerBonusLevel);
        Material* below = level->getMaterial(x, y - 1, z);
        if (below->blocksMotion() || below->isLiquid()) {
            level->setTileAndUpdate(x, y, z, Tile::water_Id);
        }
    }
}

int IceTile::getResourceCount(Random* random) { return 0; }

void IceTile::tick(Level* level, int x, int y, int z, Random* random) {
    if (level->getBrightness(LightLayer::Block, x, y, z) >
        11 - Tile::lightBlock[id]) {
        if (level->dimension->ultraWarm) {
            level->removeTile(x, y, z);
            return;
        }
        this->spawnResources(level, x, y, z, level->getData(x, y, z), 0);
        level->setTileAndUpdate(x, y, z, Tile::calmWater_Id);
    }
}

bool IceTile::shouldTileTick(Level* level, int x, int y, int z) {
    return level->getBrightness(LightLayer::Block, x, y, z) >
           11 - Tile::lightBlock[id];
}

int IceTile::getPistonPushReaction() { return Material::PUSH_NORMAL; }
