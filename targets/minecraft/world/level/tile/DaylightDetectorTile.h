#pragma once

#include <memory>

#include "BaseEntityTile.h"

class Icon;
class Level;

class DaylightDetectorTile : public BaseEntityTile {
    friend class ChunkRebuildData;

private:
    Icon* icons[2];

public:
    DaylightDetectorTile(int id);

    virtual void updateDefaultShape();  // 4J Added override
    virtual void updateShape(LevelSource* level, int x, int y, int z,
                             int forceData = -1,
                             std::shared_ptr<TileEntity> forceEntity =
                                 std::shared_ptr<TileEntity>());
    virtual int getSignal(LevelSource* level, int x, int y, int z, int dir);
    virtual void tick(Level* level, int x, int y, int z, Random* random);
    virtual void neighborChanged(Level* level, int x, int y, int z, int type);
    virtual void onPlace(Level* level, int x, int y, int z);
    virtual void updateSignalStrength(Level* level, int x, int y, int z);
    virtual bool isCubeShaped();
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool isSignalSource();
    virtual std::shared_ptr<TileEntity> newTileEntity(Level* level);
    virtual Icon* getTexture(int face, int data);
    virtual void registerIcons(IconRegister* iconRegister);
};