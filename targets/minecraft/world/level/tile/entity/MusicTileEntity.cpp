#include "MusicTileEntity.h"

#include <memory>
#include <string>

#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

MusicTileEntity::MusicTileEntity() : TileEntity() {
    note = 0;

    on = false;
}

void MusicTileEntity::save(CompoundTag* tag) {
    TileEntity::save(tag);
    tag->putByte("note", note);
}

void MusicTileEntity::load(CompoundTag* tag) {
    TileEntity::load(tag);
    note = tag->getByte("note");
    if (note < 0) note = 0;
    if (note > 24) note = 24;
}

void MusicTileEntity::tune() {
    note = (uint8_t)((note + 1) % 25);
    setChanged();
}

void MusicTileEntity::playNote(Level* level, int x, int y, int z) {
    if (level->getMaterial(x, y + 1, z) != Material::air) return;

    Material* m = level->getMaterial(x, y - 1, z);

    int i = 0;
    if (m == Material::stone) i = 1;
    if (m == Material::sand) i = 2;
    if (m == Material::glass) i = 3;
    if (m == Material::wood) i = 4;

    level->tileEvent(x, y, z, Tile::noteblock_Id, i, note);
}

// 4J Added
std::shared_ptr<TileEntity> MusicTileEntity::clone() {
    std::shared_ptr<MusicTileEntity> result =
        std::make_shared<MusicTileEntity>();
    TileEntity::clone(result);

    result->note = note;
    return result;
}
