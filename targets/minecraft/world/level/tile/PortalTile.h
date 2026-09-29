#pragma once
#include <memory>
#include <optional>

#include "HalfTransparentTile.h"

class Random;
class Level;

class PortalTile : public HalfTransparentTile {
public:
    PortalTile(int id);
    virtual void tick(Level* level, int x, int y, int z, Random* random);
    virtual std::optional<AABB> getAABB(Level* level, int x, int y, int z);
    virtual void updateShape(
        LevelSource* level, int x, int y, int z, int forceData = -1,
        std::shared_ptr<TileEntity> forceEntity = std::shared_ptr<
            TileEntity>());  // 4J added forceData, forceEntity param
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool isCubeShaped();
    virtual bool trySpawnPortal(Level* level, int x, int y, int z,
                                bool actuallySpawn);
    virtual void neighborChanged(Level* level, int x, int y, int z, int type);
    virtual bool shouldRenderFace(LevelSource* level, int x, int y, int z,
                                  int face);
    virtual int getResourceCount(Random* random);
    virtual int getRenderLayer();
    virtual void entityInside(Level* level, int x, int y, int z,
                              std::shared_ptr<Entity> entity);
    virtual void animateTick(Level* level, int xt, int yt, int zt,
                             Random* random);
    virtual int cloneTileId(Level* level, int x, int y, int z);
    virtual bool mayPick();  // 4J Added override
};
