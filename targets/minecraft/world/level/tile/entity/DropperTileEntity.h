#pragma once

#include <string>

#include "DispenserTileEntity.h"
#include "java/Class.h"

class TileEntity;

class DropperTileEntity : public DispenserTileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_DROPPERTILEENTITY; }
    static TileEntity* create() { return new DropperTileEntity(); }
    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

public:
    std::string getName();
};