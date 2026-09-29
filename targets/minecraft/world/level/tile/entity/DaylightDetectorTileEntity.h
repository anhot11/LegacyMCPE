#pragma once

#include "java/Class.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"

class DaylightDetectorTileEntity : public TileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_DAYLIGHTDETECTORTILEENTITY; }
    static TileEntity* create() { return new DaylightDetectorTileEntity(); }

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

public:
    DaylightDetectorTileEntity();

    void tick();
};