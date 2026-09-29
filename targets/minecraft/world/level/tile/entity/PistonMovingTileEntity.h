#pragma once
#include <memory>
#include <optional>

#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "minecraft/world/phys/AABB.h"

class PistonPieceEntity;
class Level;
class LevelSource;

class PistonMovingPiece : public BaseEntityTile {
public:
    PistonMovingPiece(int id);

protected:
    virtual std::shared_ptr<TileEntity> newTileEntity(Level* level);

public:
    virtual void onPlace(Level* level, int x, int y, int z);
    virtual void onRemove(Level* level, int x, int y, int z, int id, int data);
    virtual bool mayPlace(Level* level, int x, int y, int z);
    virtual bool mayPlace(Level* level, int x, int y, int z, int face);
    virtual int getRenderShape();
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool isCubeShaped();
    virtual bool use(Level* level, int x, int y, int z,
                     std::shared_ptr<Player> player, int clickedFace,
                     float clickX, float clickY, float clickZ,
                     bool soundOnly = false);  // 4J added soundOnly param;
    virtual int getResource(int data, Random* random, int playerBonusLevel);
    virtual void spawnResources(Level* level, int x, int y, int z, int data,
                                float odds, int playerBonus);
    virtual void neighborChanged(Level* level, int x, int y, int z, int type);
    static std::shared_ptr<TileEntity> newMovingPieceEntity(
        int block, int data, int facing, bool extending, bool isSourcePiston);
    virtual std::optional<AABB> getAABB(Level* level, int x, int y, int z);
    virtual void updateShape(
        LevelSource* level, int x, int y, int z, int forceData = -1,
        std::shared_ptr<TileEntity> forceEntity = std::shared_ptr<
            TileEntity>());  // 4J added forceData, forceEntity param

    std::optional<AABB> getAABB(Level* level, int x, int y, int z, int tile,
                                float progress, int facing);

private:
    std::shared_ptr<PistonPieceEntity> getEntity(LevelSource* level, int x,
                                                 int y, int z);

public:
    virtual int cloneTileId(Level* level, int x, int y, int z);
    void registerIcons(IconRegister* iconRegister);
};
