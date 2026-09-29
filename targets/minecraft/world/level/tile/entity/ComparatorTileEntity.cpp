#include "ComparatorTileEntity.h"

#include <memory>
#include <string>

#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

void ComparatorTileEntity::save(CompoundTag* tag) {
    TileEntity::save(tag);
    tag->putInt("OutputSignal", output);
}

void ComparatorTileEntity::load(CompoundTag* tag) {
    TileEntity::load(tag);
    output = tag->getInt("OutputSignal");
}

int ComparatorTileEntity::getOutputSignal() { return output; }

void ComparatorTileEntity::setOutputSignal(int value) { output = value; }

// 4J Added
std::shared_ptr<TileEntity> ComparatorTileEntity::clone() {
    std::shared_ptr<ComparatorTileEntity> result =
        std::make_shared<ComparatorTileEntity>();
    TileEntity::clone(result);

    result->output = output;

    return result;
}