#pragma once

#include <stdint.h>

#include "TileEntity.h"
#include "java/Class.h"

class Level;

class MusicTileEntity : public TileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_MUSICTILEENTITY; }
    static TileEntity* create() { return new MusicTileEntity(); }

public:
    uint8_t note;

    bool on;

    MusicTileEntity();

    virtual void save(CompoundTag* tag);
    virtual void load(CompoundTag* tag);
    void tune();
    void playNote(Level* level, int x, int y, int z);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};
