#include "SkullTileEntity.h"

#include <cstdint>
#include <memory>

#include "minecraft/network/packet/TileEntityDataPacket.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

SkullTileEntity::SkullTileEntity() {
    skullType = 0;
    rotation = 0;
    extraType = "";
}

void SkullTileEntity::save(CompoundTag* tag) {
    TileEntity::save(tag);
    tag->putByte("SkullType", static_cast<std::uint8_t>(skullType & 0xff));
    tag->putByte("Rot", static_cast<std::uint8_t>(rotation & 0xff));
    tag->putString("ExtraType", extraType);
}

void SkullTileEntity::load(CompoundTag* tag) {
    TileEntity::load(tag);
    skullType = tag->getByte("SkullType");
    rotation = tag->getByte("Rot");
    if (tag->contains("ExtraType")) extraType = tag->getString("ExtraType");
}

std::shared_ptr<Packet> SkullTileEntity::getUpdatePacket() {
    CompoundTag* tag = new CompoundTag();
    save(tag);
    return std::make_shared<TileEntityDataPacket>(
        x, y, z, TileEntityDataPacket::TYPE_SKULL, tag);
}

void SkullTileEntity::setSkullType(int skullType, const std::string& extra) {
    this->skullType = skullType;
    this->extraType = extra;
}

int SkullTileEntity::getSkullType() { return skullType; }

int SkullTileEntity::getRotation() { return rotation; }

void SkullTileEntity::setRotation(int rot) { rotation = rot; }

std::string SkullTileEntity::getExtraType() { return extraType; }

// 4J Added
std::shared_ptr<TileEntity> SkullTileEntity::clone() {
    std::shared_ptr<SkullTileEntity> result =
        std::make_shared<SkullTileEntity>();
    TileEntity::clone(result);

    result->skullType = skullType;
    result->rotation = rotation;
    result->extraType = extraType;
    return result;
}