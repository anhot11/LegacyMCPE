#pragma once

#include <string>

#include "TileEntity.h"
#include "java/Class.h"

class SkullTileEntity : public TileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_SKULLTILEENTITY; }
    static TileEntity* create() { return new SkullTileEntity(); }

public:
    static const int TYPE_SKELETON = 0;
    static inline constexpr int TYPE_WITHER = 1;
    static const int TYPE_ZOMBIE = 2;
    static const int TYPE_CHAR = 3;
    static const int TYPE_CREEPER = 4;

private:
    int skullType;
    int rotation;
    std::string extraType;

public:
    SkullTileEntity();

    void save(CompoundTag* tag);
    void load(CompoundTag* tag);
    std::shared_ptr<Packet> getUpdatePacket();
    void setSkullType(int skullType, const std::string& extra);
    int getSkullType();
    int getRotation();
    void setRotation(int rot);
    std::string getExtraType();

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};