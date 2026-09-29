#include "minecraft/StaticConstructors.h"

#include "minecraft/SharedConstants.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/server/level/ServerLevel.h"
#include "minecraft/stats/CommonStats.h"
#include "minecraft/stats/GenericStats.h"
#include "minecraft/stats/Stats.h"
#include "minecraft/world/effect/MobEffect.h"
#include "minecraft/world/entity/EntityIO.h"
#include "minecraft/world/entity/MobCategory.h"
#include "minecraft/world/entity/monster/EnderMan.h"
#include "minecraft/world/entity/npc/Villager.h"
#include "minecraft/world/item/HatchetItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/PickaxeItem.h"
#include "minecraft/world/item/ShovelItem.h"
#include "minecraft/world/item/alchemy/PotionBrewing.h"
#include "minecraft/world/item/crafting/FurnaceRecipes.h"
#include "minecraft/world/item/crafting/Recipes.h"
#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/level/LevelSettings.h"
#include "minecraft/world/level/LevelType.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/BlockReplacements.h"
#include "minecraft/world/level/chunk/CompressedTileStorage.h"
#include "minecraft/world/level/chunk/LevelChunk.h"
#include "minecraft/world/level/chunk/SparseDataStorage.h"
#include "minecraft/world/level/chunk/SparseLightStorage.h"
#include "minecraft/world/level/chunk/storage/McRegionChunkStorage.h"
#include "minecraft/world/level/levelgen/structure/MineShaftPieces.h"
#include "minecraft/world/level/levelgen/structure/RandomScatteredLargeFeature.h"
#include "minecraft/world/level/levelgen/structure/StrongholdFeature.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"
#include "minecraft/world/level/levelgen/structure/VillageFeature.h"
#include "minecraft/world/level/levelgen/structure/VillagePieces.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/material/MaterialColor.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/BeaconTileEntity.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"

void Minecraft_RunStaticCtors() {
    // The ordering of these static ctors can be important. If they are within
    // statement blocks then DO NOT CHANGE the ordering - 4J Stu

    Packet::staticCtor();

    {
        MaterialColor::staticCtor();
        Material::staticCtor();
        Tile::staticCtor();
        HatchetItem::staticCtor();
        PickaxeItem::staticCtor();
        ShovelItem::staticCtor();
        BlockReplacements::staticCtor();
        Biome::staticCtor();
        MobEffect::staticCtor();
        Item::staticCtor();
        FurnaceRecipes::staticCtor();
        Recipes::staticCtor();
        GenericStats::setInstance(new CommonStats());
        Stats::staticCtor();
        // Achievements::staticCtor(); // 4J Stu - This is now called from
        // within the Stats::staticCtor()
        TileEntity::staticCtor();
        EntityIO::staticCtor();
        MobCategory::staticCtor();

        Item::staticInit();
        LevelChunk::staticCtor();

        LevelType::staticCtor();

        {
            StructureFeatureIO::staticCtor();

            MineShaftPieces::staticCtor();
            StrongholdFeature::staticCtor();
            VillagePieces::Smithy::staticCtor();
            VillageFeature::staticCtor();
            RandomScatteredLargeFeature::staticCtor();
        }
    }
    EnderMan::staticCtor();
    PotionBrewing::staticCtor();
    Enchantment::staticCtor();

    SharedConstants::staticCtor();

    ServerLevel::staticCtor();
    SparseLightStorage::staticCtor();
    CompressedTileStorage::staticCtor();
    SparseDataStorage::staticCtor();
    McRegionChunkStorage::staticCtor();
    Villager::staticCtor();
    GameType::staticCtor();
    BeaconTileEntity::staticCtor();
}
